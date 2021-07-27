#include "lval/lval_exit.h"
#include <stdlib.h>

lval *lval_exit(long exit_code) {
  lval *v = malloc(sizeof(lval));
  v->delete = lval_exit_delete;
  v->type = LVAL_EXIT;
  v->exit_code = exit_code;
  return v;
}

void lval_exit_delete(lval* v) {
  free(v);
}
