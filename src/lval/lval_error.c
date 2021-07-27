#include "lval/lval_error.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

lval *lval_error(char *format, ...) {
  lval *v = malloc(sizeof(lval));
  v->delete = lval_error_delete;
  v->type = LVAL_ERROR;

  va_list va;
  va_start(va, format);

  v->error = malloc(512);
  vsnprintf(v->error, 511, format, va);
  v->error = realloc(v->error, strlen(v->error) + 1);

  va_end(va);
  return v;
}

void lval_error_delete(lval* v) {
  free(v->error);
  free(v);
}