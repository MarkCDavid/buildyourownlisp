#include "lval/lval.h"
#include "lval/lval_lambda.h"
#include "lenv.h"
#include <stdlib.h>

lval *lval_lambda(lval *formals, lval *body) {
  lval *v = malloc(sizeof(lval));
  v->delete = lval_lambda_delete;
  v->builtin = NULL;
  v->type = LVAL_FUNCTION;
  v->environment = lenv_new();
  v->formals = formals;
  v->body = body;
  return v;
}

void lval_lambda_delete(lval* v) {
  lenv_delete(v->environment);
  lval_delete(v->formals);
  lval_delete(v->body);
  free(v);
}