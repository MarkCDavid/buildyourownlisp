#include "lval/lval_decimal.h"
#include <stdlib.h>

lval *lval_decimal(double decimal) {
  lval *v = malloc(sizeof(lval));
  v->delete = lval_decimal_delete;
  v->type = LVAL_DECIMAL;
  v->decimal = decimal;
  return v;
}

void lval_decimal_delete(lval* v) { 
  free(v);
}