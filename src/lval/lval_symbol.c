#include "lval/lval_symbol.h"
#include <stdlib.h>
#include <string.h>

lval *lval_symbol(char *symbol) {
  lval *v = malloc(sizeof(lval));
  v->delete = lval_symbol_delete;
  v->type = LVAL_SYMBOL;
  v->symbol = malloc(strlen(symbol) + 1);
  strcpy(v->symbol, symbol);
  return v;
}

void lval_symbol_delete(lval* v) {
  free(v->symbol);
  free(v);
}