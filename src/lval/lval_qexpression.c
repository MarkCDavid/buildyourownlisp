#include "lval/lval_qexpression.h"
#include "lval/lval.h"
#include <stdlib.h>

lval *lval_qexpression(void) {
  lval *v = malloc(sizeof(lval));
  v->delete = lval_qexpression_delete;
  v->copy = lval_qexpression_copy;
  v->type = LVAL_QEXPRESSION;
  v->cell = NULL;
  v->count = 0;
  return v;
}

void lval_qexpression_delete(lval *v) {
  for (int i = 0; i < v->count; i++) {
    lval_delete(v->cell[i]);
  }
  free(v->cell);
  free(v);
}

lval *lval_qexpression_copy(lval *s, lval *d) {
  d->count = s->count;
  d->cell = malloc(sizeof(lval *) * d->count);
  for (int i = 0; i < d->count; i++) {
    d->cell[i] = lval_copy(s->cell[i]);
  }
  return d;
}