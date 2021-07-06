#include "readline.h"
#include "mpc.h"
#include "grammar.h"
#include "lval.h"
#include "lenv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {

  puts("Lispy - Type 'exit' to Exit\n");

  lispy_load();

  lenv *env = lenv_new();
  lenv_add_builtins(env);

  while (1) {

    char *input = readline("lispy> ");

    if (strcmp("exit", input) == 0) {
      break;
    }

    add_history(input);

    mpc_result_t parsing_result;
    if (mpc_parse("<stdin>", input, lispy_core, &parsing_result)) {
      lval *result = lval_eval(env, lval_read(parsing_result.output));
      lval_println(result);
      lval_delete(result);
      mpc_ast_delete(parsing_result.output);
    } else {
      mpc_err_print(parsing_result.error);
      mpc_err_delete(parsing_result.error);
    }

    free(input);
  }

  lenv_delete(env);
  lispy_cleanup();

  return 0;
}
