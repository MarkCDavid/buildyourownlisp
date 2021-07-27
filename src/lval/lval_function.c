#include "lval_function.h"
#include <stdlib.h>
#include <string.h>

lval *lval_function(lbuiltin function, char *name) {
  lval *v = malloc(sizeof(lval));
  v->delete = lval_function_delete;
  v->copy = lval_function_copy;
  v->print = lval_function_print;
  v->show = lval_function_print;
  v->type = LVAL_FUNCTION;
  v->builtin = function;
  v->builtin_name = malloc(strlen(name) + 1);
  strcpy(v->builtin_name, name);
  return v;
}

void lval_function_delete(lval *v) { free(v); }

lval *lval_function_copy(lval *s, lval *d) {
  d->builtin = s->builtin;
  d->builtin_name = malloc(strlen(s->builtin_name) + 1);
  strcpy(d->builtin_name, s->builtin_name);
  return d;
}

void lval_function_print(lval *v) {
  printf("<function '%s'>", v->builtin_name);
}