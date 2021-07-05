#include "grammar.h"
#include "lval.h"
#include "mpc.h"
#include "readline.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

lval eval_operator(lval x, char *operator, lval y) {

  if (x.type == LVAL_ERR) {
    return x;
  }
  if (y.type == LVAL_ERR) {
    return y;
  }
  if (x.type != y.type) {
    if (x.type == LVAL_INT) {
      x = lval_decimal(x.integer);
    }
    if (y.type == LVAL_INT) {
      y = lval_decimal(y.integer);
    }
  }

  if (strcmp(operator, "+") == 0) {
    if (x.type == LVAL_INT) {
      return lval_integer(x.integer + y.integer);
    }
    if (x.type == LVAL_DEC) {
      return lval_decimal(x.decimal + y.decimal);
    }
  }
  if (strcmp(operator, "-") == 0) {
    if (x.type == LVAL_INT) {
      return lval_integer(x.integer - y.integer);
    }
    if (x.type == LVAL_DEC) {
      return lval_integer(x.decimal - y.decimal);
    }
  }
  if (strcmp(operator, "*") == 0) {
    if (x.type == LVAL_INT) {
      return lval_integer(x.integer * y.integer);
    }
    if (x.type == LVAL_DEC) {
      return lval_integer(x.decimal * y.decimal);
    }
  }
  if (strcmp(operator, "/") == 0) {
    if (x.type == LVAL_INT) {
      return y.integer == 0 ? lval_error(LERR_DIV_ZERO)
                            : lval_integer(x.integer / y.integer);
    }
    if (x.type == LVAL_DEC) {
      return y.decimal == 0 ? lval_error(LERR_DIV_ZERO)
                            : lval_decimal(x.decimal / y.decimal);
    }
  }
  if (strcmp(operator, "%") == 0) {
    if (x.type == LVAL_INT) {
      return y.integer == 0 ? lval_error(LERR_DIV_ZERO)
                            : lval_integer(x.integer % y.integer);
    }
    if (x.type == LVAL_DEC) {
      return lval_error(LERR_DEC_MOD);
    }
  }

  return lval_error(LERR_BAD_OP);
}

lval eval(mpc_ast_t *token) {
  if (strstr(token->tag, "decimal")) {
    errno = 0;
    double x = strtod(token->contents, NULL);
    return errno != ERANGE ? lval_decimal(x) : lval_error(LERR_BAD_NUM);
  }

  if (strstr(token->tag, "integer")) {
    errno = 0;
    long x = strtol(token->contents, NULL, 10);
    return errno != ERANGE ? lval_integer(x) : lval_error(LERR_BAD_NUM);
  }

  char *operator= token->children[1]->contents;

  lval x = eval(token->children[2]);

  if (strcmp(operator, "-") == 0 && token->children_num == 4) {
    if (x.type == LVAL_INT) {
      x = lval_integer(-x.integer);
    }
    if (x.type == LVAL_DEC) {
      x = lval_decimal(-x.decimal);
    }
  }

  for (int i = 3; strstr(token->children[i]->tag, "expression"); i++) {
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
      lval evaluation_result = eval(parsing_result.output);
      lval_println(evaluation_result);
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
