#include "grammar.h"
#include "mpc.h"
#include "readline.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double eval_operator(double x, char* operator, double y) {
  if (strcmp(operator, "+") == 0) { return x + y; }
  if (strcmp(operator, "-") == 0) { return x - y; }
  if (strcmp(operator, "*") == 0) { return x * y; }
  if (strcmp(operator, "/") == 0) { return x / y; }
  if (strcmp(operator, "min") == 0) { return x > y ? y : x; }
  if (strcmp(operator, "max") == 0) { return x > y ? x : y; }

  return 0;
}

double eval(mpc_ast_t* token) {
  if(strstr(token->tag, "number")) {
    return atof(token->contents);
  } 

  char* operator = token->children[1]->contents;

  double x = eval(token->children[2]);

  if(strcmp(operator, "-") == 0 && token->children_num == 4) {
    x = -x;
  }

  for(int i = 3; strstr(token->children[i]->tag, "expression"); i++) {
    x = eval_operator(x, operator, eval(token->children[i]));
  }

  return x;
}

int main(int argc, char **argv) {

  puts("Lispy - Type 'exit' to Exit\n");

  lispy_load();

  while (1) {

    char *input = readline("lispy> ");

    if (strcmp("exit", input) == 0) {
      break;
    }

    add_history(input);

    mpc_result_t parsing_result;
    if (mpc_parse("<stdin>", input, lispy_core, &parsing_result)) {
      double evaluation_result = eval(parsing_result.output);
      printf("%f\n", evaluation_result);
      mpc_ast_delete(parsing_result.output);
    } else {
      mpc_err_print(parsing_result.error);
      mpc_err_delete(parsing_result.error);
    }

    free(input);
  }

  lispy_cleanup();

  return 0;
}
