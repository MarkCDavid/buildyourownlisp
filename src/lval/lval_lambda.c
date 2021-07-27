#include "lval_lambda.h"
#include "lenv.h"
#include "lval/lval.h"
#include <stdlib.h>

lval *lval_lambda(lval *formals, lval *body) {
  lval *v = malloc(sizeof(lval));
  v->delete = lval_lambda_delete;
  v->copy = lval_lambda_copy;
  v->builtin = NULL;
  v->type = LVAL_FUNCTION;
  v->environment = lenv_new();
  v->formals = formals;
  v->body = body;
  return v;
}

void lval_lambda_delete(lval *v) {
  lenv_delete(v->environment);
  lval_delete(v->formals);
  lval_delete(v->body);
  free(v);
}

lval *lval_lambda_copy(lval *s, lval *d) {
  d->builtin = NULL;
  d->environment = lenv_copy(s->environment);
  d->formals = lval_copy(s->formals);
  d->body = lval_copy(s->body);
  return d;
}