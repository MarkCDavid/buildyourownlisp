#include "lval/lval_function.h"
#include <stdlib.h>
#include <string.h>

lval *lval_function(lbuiltin function, char *name) {
  lval *v = malloc(sizeof(lval));
  v->delete = lval_function_delete;
  v->type = LVAL_FUNCTION;
  v->builtin = function;
  v->builtin_name = malloc(strlen(name) + 1);
  strcpy(v->builtin_name, name);
  return v;
}

void lval_function_delete(lval* v) {
  free(v);
}