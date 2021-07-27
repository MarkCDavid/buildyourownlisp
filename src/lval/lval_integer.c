#include "lval_integer.h"
#include <stdlib.h>

lval *lval_integer(long integer) {
  lval *v = malloc(sizeof(lval));
  v->delete = lval_integer_delete;
  v->copy = lval_integer_copy;
  v->type = LVAL_INTEGER;
  v->integer = integer;
  return v;
}

void lval_integer_delete(lval *v) { free(v); }

lval *lval_integer_copy(lval *s, lval *d) {
  d->integer = s->integer;
  return d;
}