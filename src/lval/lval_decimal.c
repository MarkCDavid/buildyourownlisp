#include "lval_decimal.h"
#include <stdlib.h>

lval *lval_decimal(double decimal) {
  lval *v = malloc(sizeof(lval));
  v->delete = lval_decimal_delete;
  v->copy = lval_decimal_copy;
  v->type = LVAL_DECIMAL;
  v->decimal = decimal;
  return v;
}

void lval_decimal_delete(lval *v) { free(v); }

lval *lval_decimal_copy(lval *s, lval *d) {
  d->decimal = s->decimal;
  return d;
}