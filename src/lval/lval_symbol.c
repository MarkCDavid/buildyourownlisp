#include "lval_symbol.h"
#include "lval.h"
#include "../lenv.h"
#include <stdlib.h>
#include <string.h>

lval *lval_symbol(char *symbol) {
  lval *v = malloc(sizeof(lval));
  v->delete = lval_symbol_delete;
  v->copy = lval_symbol_copy;
  v->print = lval_symbol_print;
  v->show = lval_symbol_print;
  v->eval = lval_symbol_eval;
  v->call = NULL;
  v->type = LVAL_SYMBOL;
  v->symbol = malloc(strlen(symbol) + 1);
  strcpy(v->symbol, symbol);
  return v;
}

void lval_symbol_delete(lval *v) {
  free(v->symbol);
  free(v);
}

lval *lval_symbol_copy(lval *s, lval *d) {
  d->symbol = malloc(strlen(s->symbol) + 1);
  strcpy(d->symbol, s->symbol);
  return d;
}

void lval_symbol_print(lval *v) { printf("%s", v->symbol); }

lval *lval_symbol_eval(lenv *e, lval *v) {
  lval *r = lenv_get(e, v);
  lval_delete(v);
  return r;
}