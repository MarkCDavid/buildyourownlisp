#include "grammar.h"
#include "mpc.h"
#include "readline.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {

  puts("Lispy - Type 'exit' to Exit\n");

  lispy_load();

  while (1) {

    char *input = readline("lispy> ");

    if (strcmp("exit", input) == 0) {
      break;
    }

    add_history(input);

    mpc_result_t result;
    if (mpc_parse("<stdin>", input, lispy_core, &result)) {
      mpc_ast_print(result.output);
      mpc_ast_delete(result.output);
    } else {
      mpc_err_print(result.error);
      mpc_err_delete(result.error);
    }

    printf("%s\n", input);

    free(input);
  }

  lispy_cleanup();

  return 0;
}
