#include "builtin.h"
#include "lenv.h"
#include "ltypes.h"
#include "lval.h"
#include "macros.h"

lval *builtin_add(lenv *e, lval *v) {
  LASSERT_ARG_COUNT_AT_LEAST("+", v, 1);

  for (int i = 0; i < v->count; i++) {
    LASSERT_TYPE("+", v, i, LVAL_NUMBER);
  }

  lval *x = lval_pop(v, 0);

  while (v->count) {
    lval *y = lval_pop(v, 0);

    x = builtin_convert_to_decimal_if_required(x, y);
    y = builtin_convert_to_decimal_if_required(y, x);

    if (x->type == LVAL_INTEGER) {
      x->integer += y->integer;
    }

    if (x->type == LVAL_DECIMAL) {
      x->decimal += y->decimal;
    }
    lval_delete(y);
  }
  lval_delete(v);
  return x;
}

lval *builtin_minus(lenv *e, lval *v) {
  LASSERT_ARG_COUNT_AT_LEAST("-", v, 1);

  if (v->count == 1) {
    return builtin_negate(e, v);
  }

  return builtin_subtract(e, v);
}

lval *builtin_multiply(lenv *e, lval *v) {
  LASSERT_ARG_COUNT_AT_LEAST("*", v, 1);

  for (int i = 0; i < v->count; i++) {
    LASSERT_TYPE("*", v, i, LVAL_NUMBER);
  }

  lval *x = lval_pop(v, 0);

  while (v->count) {
    lval *y = lval_pop(v, 0);

    x = builtin_convert_to_decimal_if_required(x, y);
    y = builtin_convert_to_decimal_if_required(y, x);

    if (x->type == LVAL_INTEGER) {
      x->integer *= y->integer;
    }

    if (x->type == LVAL_DECIMAL) {
      x->decimal *= y->decimal;
    }
    lval_delete(y);
  }
  lval_delete(v);
  return x;
}

lval *builtin_divide(lenv *e, lval *v) {
  LASSERT_ARG_COUNT_AT_LEAST("/", v, 1);

  for (int i = 0; i < v->count; i++) {
    LASSERT_TYPE("/", v, i, LVAL_NUMBER);
  }

  lval *x = lval_pop(v, 0);

  while (v->count) {
    lval *y = lval_pop(v, 0);

    x = builtin_convert_to_decimal_if_required(x, y);
    y = builtin_convert_to_decimal_if_required(y, x);

    if ((y->type == LVAL_INTEGER && y->integer == 0) ||
        (y->type == LVAL_DECIMAL && y->decimal == 0)) {
      lval_delete(x);
      lval_delete(y);
      lval_delete(v);
      return lval_error("Division by zero!");
    }

    if (x->type == LVAL_INTEGER) {
      x->integer /= y->integer;
    }

    if (x->type == LVAL_DECIMAL) {
      x->decimal /= y->decimal;
    }
    lval_delete(y);
  }
  lval_delete(v);
  return x;
}

lval *builtin_modulo(lenv *e, lval *v) {
  LASSERT_ARG_COUNT_AT_LEAST("%", v, 1);

  for (int i = 0; i < v->count; i++) {
    LASSERT_TYPE("%", v, i, LVAL_NUMBER);
  }

  lval *x = lval_pop(v, 0);

  if (x->type == LVAL_DECIMAL) {
    lval_delete(x);
    lval_delete(v);
    return lval_error("Modulo operation on decimal numbers!");
  }

  while (v->count) {
    lval *y = lval_pop(v, 0);

    if (y->type == LVAL_DECIMAL) {
      lval_delete(x);
      lval_delete(y);
      lval_delete(v);
      return lval_error("Modulo operation on decimal numbers!");
    }

    if (x->type == LVAL_INTEGER) {
      x->integer %= y->integer;
    }

    if (x->type == LVAL_DECIMAL) {
      x->decimal += y->decimal;
    }
    lval_delete(y);
  }
  lval_delete(v);
  return x;
}

lval *builtin_negate(lenv *e, lval *v) {
  LASSERT_ARG_COUNT("negate", v, 1);
  LASSERT_TYPE("negate", v, 0, LVAL_NUMBER);

  lval *x = lval_pop(v, 0);
  if (x->type == LVAL_INTEGER) {
    x->integer = -x->integer;
  }

  if (x->type == LVAL_DECIMAL) {
    x->decimal = -x->decimal;
  }
  lval_delete(v);
  return x;
}

lval *builtin_subtract(lenv *e, lval *v) {
  LASSERT_ARG_COUNT_AT_LEAST("-", v, 1);

  for (int i = 0; i < v->count; i++) {
    LASSERT_TYPE("-", v, i, LVAL_NUMBER);
  }

  lval *x = lval_pop(v, 0);

  while (v->count) {
    lval *y = lval_pop(v, 0);

    x = builtin_convert_to_decimal_if_required(x, y);
    y = builtin_convert_to_decimal_if_required(y, x);

    if (x->type == LVAL_INTEGER) {
      x->integer -= y->integer;
    }

    if (x->type == LVAL_DECIMAL) {
      x->decimal -= y->decimal;
    }
    lval_delete(y);
  }
  lval_delete(v);
  return x;
}

lval *builtin_list(lenv *e, lval *v) {
  v->type = LVAL_QEXPRESSION;
  return v;
}

lval *builtin_head(lenv *e, lval *v) {
  LASSERT_ARG_COUNT("head", v, 1);
  LASSERT_TYPE("head", v, 0, LVAL_QEXPRESSION);
  LASSERT_NOT_EMPTY("head", v, 0);

  lval *r = lval_take(v, 0);
  while (r->count > 1) {
    lval_delete(lval_pop(r, 1));
  }
  return r;
}

lval *builtin_tail(lenv *e, lval *v) {
  LASSERT_ARG_COUNT("tail", v, 1);
  LASSERT_TYPE("tail", v, 0, LVAL_QEXPRESSION);
  LASSERT_NOT_EMPTY("tail", v, 0);

  lval *r = lval_take(v, 0);
  lval_delete(lval_pop(r, 0));
  return r;
}

lval *builtin_eval(lenv *e, lval *v) {
  LASSERT_ARG_COUNT("eval", v, 1);
  LASSERT_TYPE("eval", v, 0, LVAL_QEXPRESSION);

  lval *r = lval_take(v, 0);
  r->type = LVAL_SEXPRESSION;
  return lval_eval(e, r);
}

lval *builtin_join(lenv *e, lval *v) {
  for (int i = 0; i < v->count; i++) {
    LASSERT_TYPE("join", v, i, LVAL_QEXPRESSION);
  }

  lval *r = lval_pop(v, 0);
  while (v->count) {
    r = lval_join(r, lval_pop(v, 0));
  }

  lval_delete(v);
  return r;
}

lval *builtin_cons(lenv *e, lval *v) {
  LASSERT_ARG_COUNT("cons", v, 2);
  LASSERT_TYPE("cons", v, 1, LVAL_QEXPRESSION);

  lval *x = lval_pop(v, 0);
  lval *t = lval_pop(v, 0);

  t->count++;
  t->cell = realloc(t->cell, sizeof(lval *) * t->count);
  memmove(&t->cell[1], &t->cell[0], sizeof(lval *) * (t->count - 1));
  t->cell[0] = x;

  lval_delete(v);
  return t;
}

lval *builtin_len(lenv *e, lval *v) {
  LASSERT_ARG_COUNT("len", v, 1);
  LASSERT_TYPE("len", v, 0, LVAL_QEXPRESSION);

  lval *r = lval_integer(v->cell[0]->count);
  lval_delete(v);
  return r;
}

lval *builtin_init(lenv *e, lval *v) {
  LASSERT_ARG_COUNT("init", v, 1);
  LASSERT_TYPE("init", v, 0, LVAL_QEXPRESSION);

  lval *t = lval_pop(v, 0);

  lval_delete(lval_pop(t, 0));
  lval_delete(v);

  return t;
}

lval *builtin_lambda(lenv *e, lval *v) {
  LASSERT_ARG_COUNT("\\", v, 2);
  LASSERT_TYPE("\\", v, 0, LVAL_QEXPRESSION);
  LASSERT_TYPE("\\", v, 1, LVAL_QEXPRESSION);

  for (int i = 0; i < v->cell[0]->count; i++) {
    LASSERT(v, (v->cell[0]->cell[i]->type == LVAL_SYMBOL),
            "Cannot define non-symbol. Got %s, Expected %s.",
            ltype_name(v->cell[0]->cell[i]->type), ltype_name(LVAL_SYMBOL));
  }

  lval *formals = lval_pop(v, 0);
  lval *body = lval_pop(v, 0);
  lval_delete(v);

  return lval_lambda(formals, body);
}

lval *builtin_def(lenv *e, lval *v) { return builtin_variable(e, v, "def"); }

lval *builtin_put(lenv *e, lval *v) { return builtin_variable(e, v, "="); }

lval *builtin_variable(lenv *e, lval *v, char *function) {
  LASSERT_TYPE(function, v, 0, LVAL_QEXPRESSION);

  lval *symbols = v->cell[0];

  for (int i = 0; i < symbols->count; i++) {
    LASSERT_TYPE(function, v->cell[0], i, LVAL_SYMBOL);
  }
  
  if(symbols->count != v->count - 1) {
    lval* error = lval_error("Function '%s' cannot define incorrect number of values to symbols! "
    "Got %i, Expected %i.", function, symbols->count, v->count - 1);
    lval_delete(v);
    return error;
  }

  lval *builtin = lenv_has_builtins(e, symbols);
  if (builtin) {
    lval *error = lval_error("Cannot redefine builtin function '%s'.",
                             builtin->builtin_name);
    lval_delete(builtin);
    lval_delete(v);
    return error;
  }

  for (int i = 0; i < symbols->count; i++) {
    if(strcmp(function, "def") == 0) {
      lenv_def(e, symbols->cell[i], v->cell[i + 1]);
    }
    if(strcmp(function, "=") == 0) {
      lenv_put(e, symbols->cell[i], v->cell[i + 1]);
    }
  }

  lval_delete(v);
  return lval_sexpression();
}

lval *builtin_fun(lenv* e, lval *v) {
  LASSERT_TYPE("fun", v, 0, LVAL_QEXPRESSION);
  LASSERT_TYPE("fun", v, 1, LVAL_QEXPRESSION);

  lval *arguments = lval_add(lval_qexpression(), lval_pop(v, 0));
  lval *arguments_head = builtin_head(e, lval_copy(arguments));
  lval *arguments_tail =  builtin_tail(e, lval_copy(arguments));

  lval *body = lval_pop(v, 0);
  lval *lambda = lval_lambda(arguments_tail, body);

  lval *def_arguments = lval_qexpression();
  lval_add(def_arguments, arguments_head);
  lval_add(def_arguments, lambda);

  lval_delete(v);
  lval_delete(arguments);
  return builtin_def(e, def_arguments);
}

lval *builtin_convert_to_decimal_if_required(lval *t, lval *c) {
  if (t->type == LVAL_DECIMAL) {
    return t;
  }

  if (c->type == LVAL_DECIMAL) {
    lval *r = lval_decimal(t->integer);
    lval_delete(t);
    return r;
  }

  return t;
}