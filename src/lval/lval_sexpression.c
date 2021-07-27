#include "lval_sexpression.h"
#include "lval/lval.h"
#include <stdlib.h>

lval *lval_sexpression(void) {
  lval *v = malloc(sizeof(lval));
  v->delete = lval_sexpression_delete;
  v->copy = lval_sexpression_copy;
  v->print = lval_sexpression_print;
  v->show = lval_sexpression_print;
  v->type = LVAL_SEXPRESSION;
  v->cell = NULL;
  v->count = 0;
  return v;
}

void lval_sexpression_delete(lval *v) {
  for (int i = 0; i < v->count; i++) {
    lval_delete(v->cell[i]);
  }
  free(v->cell);
  free(v);
}

lval *lval_sexpression_copy(lval *s, lval *d) {
  d->count = s->count;
  d->cell = malloc(sizeof(lval *) * d->count);
  for (int i = 0; i < d->count; i++) {
    d->cell[i] = lval_copy(s->cell[i]);
  }
  return d;
}

void lval_sexpression_print(lval *v) {
putchar('(');
  for (int i = 0; i < v->count; i++) {
    lval_print(v->cell[i]);
    if (i != (v->count - 1)) {
      putchar(' ');
    }
  }
  putchar(')');
}