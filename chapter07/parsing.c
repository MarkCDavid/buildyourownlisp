#include "mpc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32

static char buffer[2048];
char* readline(char* prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char* input = malloc(strlen(buffer)+1);
  strcpy(input, buffer);
  input[strlen(input) - 1] = '\0';
  return input;
}

void add_history(char* history) {}

#else
#include <editline/readline.h>
#include <editline/history.h>
#endif

long power(long base, long degree) {
  long result = 1;
  for(int i = 0; i < degree; i++) {
    result *= base;
  }
  return result;
}


long eval_op(long x, char* op, long y) {
  if (strcmp(op, "+") == 0) { return x + y; }
  if (strcmp(op, "-") == 0) { return x - y; }
  if (strcmp(op, "*") == 0) { return x * y; }
  if (strcmp(op, "/") == 0) { return x / y; }
  if (strcmp(op, "%") == 0) { return x % y; }
  if (strcmp(op, "^") == 0) { return power(x, y); }
  if (strcmp(op, "min") == 0) { return x > y ? y : x; }
  if (strcmp(op, "max") == 0) { return x > y ? x : y; }

  return 0;
}

long eval(mpc_ast_t* t) {
  if(strstr(t->tag, "number")) {
    return atoi(t->contents);
  } 

  char* op = t->children[1]->contents;

  long x = eval(t->children[2]);

  if(strcmp(op, "-") == 0 && t->children_num == 4) {
    x = -x;
  }

  for(int i = 3; strstr(t->children[i]->tag, "expr"); i++) {
    x = eval_op(x, op, eval(t->children[i]));
  }

  return x;
}

int main(int argc, char** argv) {

  mpc_parser_t* Number = mpc_new("number");
  mpc_parser_t* Operator = mpc_new("operator");
  mpc_parser_t* Expression = mpc_new("expression");
  mpc_parser_t* Lispy = mpc_new("lispy");

  mpca_lang(MPCA_LANG_DEFAULT,
    "                                                                      \
      number     : /-?[0-9]+/ ;                                            \
      operator   : '+' | '-' | '*' | '/' | '%' | '^' | \"min\" | \"max\" ; \
      expression : <number> | '(' <operator> <expression>+ ')' ;           \
      lispy      : /^/ <operator> <expression>+ /$/ ;                      \
    ", Number, Operator, Expression, Lispy);


  puts("Lispy - Press Ctrl+c to Exit\n");

  while(1) {
    
    char* input = readline("lispy> ");

    add_history(input);

    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Lispy, &r)) {
      long result = eval(r.output);
      printf("%li\n", result);
      mpc_ast_delete(r.output);
    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }

    free(input);
  }

  mpc_cleanup(4, Number, Operator, Expression, Lispy);

  return 0;
}
