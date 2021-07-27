#include "lval_sexpression.h"
#include "lval/lval.h"
#include <stdlib.h>

lval *lval_sexpression(void) {
  lval *v = malloc(sizeof(lval));
  v->delete = lval_sexpression_delete;
  v->copy = lval_sexpression_copy;
  v->print = lval_sexpression_print;
  v->show = lval_sexpression_print;
  v->eval = lval_sexpression_eval;
  v->call = NULL;
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

lval *lval_sexpression_eval(lenv *e, lval *v) {
  for (int i = 0; i < v->count; i++) {
    v->cell[i] = lval_eval(e, v->cell[i]);
  }
  for (int i = 0; i < v->count; i++) {
    if (v->cell[i]->type == LVAL_ERROR) {
      return lval_take(v, i);
    }
  }
  if (v->count == 0) {
    return v;
  }

  if (v->count == 1) {
    return lval_take(v, 0);
  }

  lval *f = lval_pop(v, 0);
  if (f->type != LVAL_FUNCTION) {
    lval_delete(f);
    lval_delete(v);
    return lval_error(
        "S-Expression starts with incorrect type! Got %s, Expected %s.",
        ltype_name(f->type), ltype_name(LVAL_FUNCTION));
  }

  printf("lval_call "); lval_println(f);
  lval *result = lval_call(e, f, v);
  printf("lval_result "); lval_println(result);
  printf("lval_exit "); lval_println(f);
  lval_delete(f);
  return result;
}