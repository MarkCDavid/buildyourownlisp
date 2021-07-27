#include "lval_qexpression.h"
#include "lval/lval.h"
#include <stdlib.h>

lval *lval_qexpression(void) {
  lval *v = malloc(sizeof(lval));
  v->delete = lval_qexpression_delete;
  v->copy = lval_qexpression_copy;
  v->print = lval_qexpression_print;
  v->show = lval_qexpression_print;
  v->eval = lval_qexpression_eval;
  v->call = NULL;
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

void lval_qexpression_print(lval *v) {
putchar('{');
  for (int i = 0; i < v->count; i++) {
    lval_print(v->cell[i]);
    if (i != (v->count - 1)) {
      putchar(' ');
    }
  }
  putchar('}');
}
lval *lval_qexpression_eval(lenv* e, lval *v) { return v; }