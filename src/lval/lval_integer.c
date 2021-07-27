#include "lval/lval_integer.h"
#include <stdlib.h>

lval *lval_integer(long integer) {
  lval *v = malloc(sizeof(lval));
  v->delete = lval_integer_delete;
  v->type = LVAL_INTEGER;
  v->integer = integer;
  return v;
}

void lval_integer_delete(lval* v) {
  free(v);
}