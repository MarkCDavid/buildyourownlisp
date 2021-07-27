#include "readline.h"
#include "mpc.h"
#include "grammar.h"
#include "lval/lval.h"
#include "lenv.h"
#include "builtin.h"
#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {

  lispy_load();

  long exit_code = 0;
  lenv *env = lenv_new();
  lenv_add_builtins(env);

  if(argc == 1) {
    
    puts("Lispy - Type 'exit' to Exit\n");

    while (1) {

      char *input = readline("lispy> ");

      add_history(input);

      mpc_result_t parsing_result;
      if (mpc_parse("<stdin>", input, lispy_core, &parsing_result)) {
        lval *result = lval_eval(env, lval_read(parsing_result.output));

        if(result->type == LVAL_EXIT) {
          exit_code = result->exit_code;
          break;
        }

        lval_println(result);
        lval_delete(result);
        mpc_ast_delete(parsing_result.output);
      } else {
        mpc_err_print(parsing_result.error);
        mpc_err_delete(parsing_result.error);
      }

      free(input);
    }
  }

  if(argc >= 2) {
    for(int i = 1; i < argc; i++) {
      lval* arguments = lval_add(lval_sexpression(), lval_string(argv[i]));
      lval* result = builtin_load(env, arguments);
      if(result->type == LVAL_ERROR) {
        lval_print(result);
      }
      lval_delete(result);
    }
  }

  lenv_delete(env);
  lispy_cleanup();

  return exit_code;
}
