#include "lval/lval_file.h"
#include <stdlib.h>
#include <string.h>

lval *lval_file(FILE *file, char *file_name, char *mode) {
  lval *v = malloc(sizeof(lval));
  v->delete = lval_file_delete;
  v->type = LVAL_FILE;
  v->file = file;
  v->file_name = malloc(strlen(file_name) + 1);
  strcpy(v->file_name, file_name);
  v->mode = malloc(strlen(mode) + 1);
  strcpy(v->mode, mode);
  return v;
}

void lval_file_delete(lval* v) {
  free(v->file_name);
  free(v->mode);
  free(v);
}