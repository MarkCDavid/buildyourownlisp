#include "lval_lambda.h"
#include "lenv.h"
#include "lval/lval.h"
#include "../builtin.h"
#include <stdlib.h>

lval *lval_lambda(lval *formals, lval *body) {
  lval *v = malloc(sizeof(lval));
  v->delete = lval_lambda_delete;
  v->copy = lval_lambda_copy;
  v->print = lval_lambda_print;
  v->show = lval_lambda_print;
  v->eval = lval_lambda_eval;
  v->call = lval_lambda_call;
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

void lval_lambda_print(lval *v) {
  printf("{\\ ");
  lval_print(v->formals);
  putchar(' ');
  lval_print(v->body);
  putchar(')');
}

lval *lval_lambda_eval(lenv* e, lval *v) { return v; }

lval *lval_lambda_call(lenv* e, lval *f, lval *a) {
  printf("lambda_call\n"); 

  int given = a->count;
  int total = f->formals->count;

  while (a->count) {
    if (f->formals->count == 0) {
      lval_delete(a);
      return lval_error("Function passed too many arguments. "
                        "Got %i, Expected %i.",
                        given, total);
    }

    lval *symbol = lval_pop(f->formals, 0);

    if (strcmp(symbol->symbol, "&") == 0) {
      if (f->formals->count != 1) {
        lval_delete(a);
        return lval_error("Function format invalid. "
                          "Symbol '&' not follow by single symbol.");
      }

      lval *nsymbols = lval_pop(f->formals, 0);
      lenv_put(f->environment, nsymbols, builtin_list(e, a));
      lval_delete(symbol);
      lval_delete(nsymbols);
      break;
    }

    lval *value = lval_pop(a, 0);
    lenv_put(f->environment, symbol, value);
    lval_delete(symbol);
    lval_delete(value);
  }

  lval_delete(a);

  if (f->formals->count > 0 && strcmp(f->formals->cell[0]->symbol, "&") == 0) {
    if (f->formals->count != 2) {
      return lval_error("Function format invalid. "
                        "Symbol '&' not follow by single symbol.");
    }
    lval_delete(lval_pop(f->formals, 0));

    lval *symbol = lval_pop(f->formals, 0);
    lval *value = lval_qexpression();

    lenv_put(f->environment, symbol, value);
    lval_delete(symbol);
    lval_delete(value);
  }

  
  if (f->formals->count == 0) {
    f->environment->parent = e;
    return builtin_eval(f->environment, lval_add(lval_sexpression(), lval_copy(f->body)));
  } else {
    return lval_copy(f);
  }
 }