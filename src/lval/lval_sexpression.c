#include "lval/lval.h"
#include "lval/lval_sexpression.h"
#include <stdlib.h>

lval *lval_sexpression(void) {
  lval *v = malloc(sizeof(lval));
  v->delete = lval_sexpression_delete;
  v->type = LVAL_SEXPRESSION;
  v->cell = NULL;
  v->count = 0;
  return v;
}

void lval_sexpression_delete(lval* v) {
  for (int i = 0; i < v->count; i++) {
    lval_delete(v->cell[i]);
  }
  free(v->cell);
  free(v);
}