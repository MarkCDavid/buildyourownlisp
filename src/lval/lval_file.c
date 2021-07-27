#include "lval_file.h"
#include <stdlib.h>
#include <string.h>

lval *lval_file(FILE *file, char *file_name, char *mode) {
  lval *v = malloc(sizeof(lval));
  v->delete = lval_file_delete;
  v->copy = lval_file_copy;
  v->print = lval_file_print;
  v->show = lval_file_print;
  v->type = LVAL_FILE;
  v->file = file;
  v->file_name = malloc(strlen(file_name) + 1);
  strcpy(v->file_name, file_name);
  v->mode = malloc(strlen(mode) + 1);
  strcpy(v->mode, mode);
  return v;
}

void lval_file_delete(lval *v) {
  free(v->file_name);
  free(v->mode);
  free(v);
}

lval *lval_file_copy(lval *s, lval *d) {
  d->file = s->file;
  d->file_name = malloc(strlen(s->file_name) + 1);
  strcpy(d->file_name, s->file_name);
  d->mode = malloc(strlen(s->mode) + 1);
  strcpy(d->mode, s->mode);
  return d;
}

void lval_file_print(lval *v) {
  printf("<file %s (%s)>", v->file_name, v->mode);
}