#include "lval/lval_error.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

lval *lval_error(char *format, ...) {
  lval *v = malloc(sizeof(lval));
  v->delete = lval_error_delete;
  v->copy = lval_error_copy;
  v->type = LVAL_ERROR;

  va_list va;
  va_start(va, format);

  v->error = malloc(512);
  vsnprintf(v->error, 511, format, va);
  v->error = realloc(v->error, strlen(v->error) + 1);
  va_end(va);
  return v;
}

void lval_error_delete(lval *v) {
  free(v->error);
  free(v);
}

lval *lval_error_copy(lval *s, lval *d) {
  d->error = malloc(strlen(s->error) + 1);
  strcpy(d->error, s->error);
  return d;
}