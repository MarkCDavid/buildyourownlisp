#include "lval_string.h"
#include "../mpc.h"
#include <stdlib.h>
#include <string.h>

lval *lval_string(char *string) {
  lval *v = malloc(sizeof(lval));
  v->delete = lval_string_delete;
  v->copy = lval_string_copy;
  v->print = lval_string_print;
  v->show = lval_string_show;
  v->type = LVAL_STRING;
  v->string = malloc(strlen(string) + 1);
  strcpy(v->string, string);
  return v;
}

void lval_string_delete(lval *v) {
  free(v->string);
  free(v);
}

lval *lval_string_copy(lval *s, lval *d) {
  d->string = malloc(strlen(s->string) + 1);
  strcpy(d->string, s->string);
  return d;
}

void lval_string_print(lval *v) {
  char *escaped_string = malloc(strlen(v->string) + 1);
  strcpy(escaped_string, v->string);
  escaped_string = mpcf_escape(escaped_string);
  printf("\"%s\"", escaped_string);
  free(escaped_string);
}

void lval_string_show(lval *v) { printf("\"%s\"", v->string); }