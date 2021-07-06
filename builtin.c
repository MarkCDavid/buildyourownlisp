#include "macros.h"
#include "builtin.h"

lval *builtin(lval *v, char *function) {
  if (strcmp("list", function) == 0) {
    return builtin_list(v);
  }
  if (strcmp("head", function) == 0) {
    return builtin_head(v);
  }
  if (strcmp("tail", function) == 0) {
    return builtin_tail(v);
  }
  if (strcmp("join", function) == 0) {
    return builtin_join(v);
  }
  if (strcmp("eval", function) == 0) {
    return builtin_eval(v);
  }
  if (strcmp("cons", function) == 0) {
    return builtin_cons(v);
  }
  if (strcmp("len", function) == 0) {
    return builtin_len(v);
  }
  if (strcmp("init", function) == 0) {
    return builtin_init(v);
  }
  if (strstr("+-/*%", function)) {
    return builtin_operator(v, function);
  }

  lval_delete(v);
  return lval_error("Unknown function!");
}

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

  while (v->count > 0) {
    if (x->type == LVAL_ERROR) {
      lval_delete(x);
      break;
    }

    lval *y = lval_pop(v, 0);

    if (x->type != y->type) {
      x = builtin_convert_to_decimal(x);
      y = builtin_convert_to_decimal(y);
    }

    if (strcmp(operator, "+") == 0) {
      x = builtin_add(x, y);
    }
    if (strcmp(operator, "-") == 0) {
      x = builtin_subtract(x, y);
    }
    if (strcmp(operator, "*") == 0) {
      x = builtin_multiply(x, y);
    }
    if (strcmp(operator, "/") == 0) {
      x = builtin_divide(x, y);
    }
    if (strcmp(operator, "%") == 0) {
      x = builtin_modulo(x, y);
    }

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

lval *builtin_add(lval *x, lval *y) {
  if (x->type == LVAL_INTEGER) {
    x->integer += y->integer;
  }

  if (x->type == LVAL_DECIMAL) {
    x->decimal += y->decimal;
  }

  return x;
}

lval *builtin_subtract(lval *x, lval *y) {
  if (x->type == LVAL_INTEGER) {
    x->integer -= y->integer;
  }

  if (x->type == LVAL_DECIMAL) {
    x->decimal -= y->decimal;
  }

  return x;
}

lval *builtin_multiply(lval *x, lval *y) {
  if (x->type == LVAL_INTEGER) {
    x->integer *= y->integer;
  }

  if (x->type == LVAL_DECIMAL) {
    x->decimal *= y->decimal;
  }

  return x;
}

lval *builtin_divide(lval *x, lval *y) {
  if ((y->type == LVAL_INTEGER && y->integer == 0) ||
      (y->type == LVAL_DECIMAL && y->decimal == 0)) {
    return lval_error("Division by zero!");
  }

  if (x->type == LVAL_INTEGER) {
    x->integer /= y->integer;
  }

  if (x->type == LVAL_DECIMAL) {
    x->decimal /= y->decimal;
  }

  return x;
}

lval *builtin_modulo(lval *x, lval *y) {
  if (x->type == LVAL_INTEGER) {
    x->integer %= y->integer;
  }

  if (x->type == LVAL_DECIMAL) {
    return lval_error("Modulo operation on decimal numbers!");
  }

  return x;
}


lval *builtin_list(lval *x) {
  x->type = LVAL_QEXPRESSION;
  return x;
}

lval *builtin_head(lval *x) {
  LASSERT_ARG_COUNT(x, 1, "Function 'head' passed too many arguments!");
  LASSERT_TYPE(x, 0, LVAL_QEXPRESSION, "Function 'head' passed incorrect type!");
  LASSERT_NOT_EMPTY(x, 0, "Function 'head' passed {}!");

  lval *r = lval_take(x, 0);
  while (r->count > 1) { 
    lval_delete(lval_pop(r, 1));
  }
  return r;
}

lval *builtin_tail(lval *x) {
  LASSERT_ARG_COUNT(x, 1, "Function 'tail' passed too many arguments!");
  LASSERT_TYPE(x, 0, LVAL_QEXPRESSION, "Function 'tail' passed incorrect type!");
  LASSERT_NOT_EMPTY(x, 0, "Function 'tail' passed {}!");

  lval *r = lval_take(x, 0);
  lval_delete(lval_pop(r, 0));
  return r;
}

lval *builtin_eval(lval *x) {
  LASSERT_ARG_COUNT(x, 1, "Function 'eval' passed too many arguments!");
  LASSERT_TYPE(x, 0, LVAL_QEXPRESSION, "Function 'eval' passed incorrect type!");

  lval *r = lval_take(x, 0);
  r->type = LVAL_SEXPRESSION;
  return lval_eval(r);

}

lval *builtin_join(lval *x) {
  for(int i = 0; i < x->count; i++) {
    LASSERT_TYPE(x, i, LVAL_QEXPRESSION, "Function 'join' passed incorrect type!");
  }

  lval* r = lval_pop(x, 0);
  while(x->count) {
    r = lval_join(r, lval_pop(x, 0));
  }

  lval_delete(x);
  return r;
}

lval *builtin_cons(lval *x) {
  LASSERT_ARG_COUNT(x, 2, "Function 'cons' passed incorrect number of arguments!");
  LASSERT_TYPE(x, 1, LVAL_QEXPRESSION, "Function 'cons' passed incorrect type!");

  lval *v = lval_pop(x, 0);
  lval *t = lval_pop(x, 0);

  t->count++;
  t->cell = realloc(t->cell, sizeof(lval *) * t->count);
  memmove(&t->cell[1], &t->cell[0], sizeof(lval *) * (t->count - 1));
  t->cell[0] = v;

  lval_delete(x);
  return t;
  
}

lval *builtin_len(lval *x) {
  LASSERT_ARG_COUNT(x, 1, "Function 'len' passed incorrect number of arguments!");
  LASSERT_TYPE(x, 0, LVAL_QEXPRESSION, "Function 'len' passed incorrect type!");

  lval *r = lval_integer(x->cell[0]->count);
  lval_delete(x);
  return r;
}

lval *builtin_init(lval *x) {
  LASSERT_ARG_COUNT(x, 1, "Function 'init' passed incorrect number of arguments!");
  LASSERT_TYPE(x, 0, LVAL_QEXPRESSION, "Function 'init' passed incorrect type!");

  lval *t = lval_pop(x, 0); 

  lval_delete(lval_pop(t, 0));
  lval_delete(x);

  return t;
}

lval *builtin_convert_to_decimal(lval *v) {
  if (v->type == LVAL_DECIMAL) {
    return v;
  }

  lval *r = lval_decimal(v->integer);
  lval_delete(v);
  return r;
}