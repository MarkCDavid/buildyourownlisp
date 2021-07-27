#include "lval/lval_ok.h"
#include <stdlib.h>
#include <string.h>

lval *lval_ok(void) {
  lval *v = malloc(sizeof(lval));
  v->delete = lval_ok_delete;
  v->type = LVAL_OK;
  return v;
}

void lval_ok_delete(lval* v) {
  free(v);
}