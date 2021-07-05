#include "builtin.h"

lval *builtin_operator(lval *v, char *operator) {
  for (int i = 0; i < v->count; i++) {
    if (v->cell[i]->type != LVAL_INTEGER && v->cell[i]->type != LVAL_DECIMAL) {
      lval_delete(v);
      return lval_error("Cannot operate on non-number!");
    }
  }

  lval *x = lval_pop(v, 0);

  if ((strcmp(operator, "-") == 0) && v->count == 0) {
    x = builtin_negate(x);
  }

  while(v -> count > 0) {
    if(x->type == LVAL_ERROR) {
      lval_delete(x);
      break;
    }

    lval *y = lval_pop(v, 0);

    if(x->type != y->type) {
      x = builtin_convert_to_decimal(x);
      y = builtin_convert_to_decimal(y);
    }

    if(strcmp(operator, "+") == 0) { x = builtin_add(x, y); }
    if(strcmp(operator, "-") == 0) { x = builtin_subtract(x, y); }
    if(strcmp(operator, "*") == 0) { x = builtin_multiply(x, y); }
    if(strcmp(operator, "/") == 0) { x = builtin_divide(x, y); }
    if(strcmp(operator, "%") == 0) { x = builtin_modulo(x, y); }

    lval_delete(y);
  }

  lval_delete(v);
  return x;
}

lval *builtin_negate(lval *x) {
  if (x->type == LVAL_INTEGER) {
    x->integer = -x->integer;
  }
  if (x->type == LVAL_DECIMAL) {
    x->decimal = -x->decimal;
  }
  return x;
}

lval* builtin_add(lval* x, lval* y) {
  if(x->type == LVAL_INTEGER) {
    x->integer += y->integer;
  }

  if(x->type == LVAL_DECIMAL) {
    x->decimal += y->decimal;
  }

  return x;
}

lval* builtin_subtract(lval* x, lval* y){
  if(x->type == LVAL_INTEGER) {
    x->integer -= y->integer;
  }

  if(x->type == LVAL_DECIMAL) {
    x->decimal -= y->decimal;
  }

  return x;
}

lval* builtin_multiply(lval* x, lval* y){
  if(x->type == LVAL_INTEGER) {
    x->integer *= y->integer;
  }

  if(x->type == LVAL_DECIMAL) {
    x->decimal *= y->decimal;
  }

  return x;
}

lval* builtin_divide(lval* x, lval* y){
  if((y->type == LVAL_INTEGER && y->integer == 0) ||
     (y->type == LVAL_DECIMAL && y->decimal == 0)) {
    return lval_error("Division by zero!");     
  }

  if(x->type == LVAL_INTEGER) {
    x->integer /= y->integer;
  }

  if(x->type == LVAL_DECIMAL) {
    x->decimal /= y->decimal;
  }

  return x;
}

lval* builtin_modulo(lval* x, lval* y){
  if(x->type == LVAL_INTEGER) {
    x->integer %= y->integer;
  }

  if(x->type == LVAL_DECIMAL) {
    return lval_error("Modulo operation on decimal numbers!");
  }

  return x;
}

lval *builtin_convert_to_decimal(lval *v) {
  if(v->type == LVAL_DECIMAL) {
    return v;
  }

  lval* r = lval_decimal(v->integer);
  lval_delete(v);
  return r;
}