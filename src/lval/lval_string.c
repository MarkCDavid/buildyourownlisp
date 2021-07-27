#include "lval/lval_string.h"
#include <stdlib.h>
#include <string.h>

lval *lval_string(char *string) {
  lval *v = malloc(sizeof(lval));
  v->delete = lval_string_delete;
  v->type = LVAL_STRING;
  v->string = malloc(strlen(string) + 1);
  strcpy(v->string, string);
  return v;
}

void lval_string_delete(lval* v) { 
  free(v->string);
  free(v);
}