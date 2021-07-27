#include "lval/lval.h"
#include "lval/lval_qexpression.h"
#include <stdlib.h>

lval *lval_qexpression(void) {
  lval *v = malloc(sizeof(lval));
  v->delete = lval_qexpression_delete;
  v->type = LVAL_QEXPRESSION;
  v->cell = NULL;
  v->count = 0;
  return v;
}

void lval_qexpression_delete(lval* v) {
  for (int i = 0; i < v->count; i++) {
    lval_delete(v->cell[i]);
  }
  free(v->cell);
  free(v);
}