#include "lval_exit.h"
#include <stdlib.h>

lval *lval_exit(long exit_code) {
  lval *v = malloc(sizeof(lval));
  v->delete = lval_exit_delete;
  v->copy = lval_exit_copy;
  v->print = lval_exit_print;
  v->show = lval_exit_print;
  v->type = LVAL_EXIT;
  v->exit_code = exit_code;
  return v;
}

void lval_exit_delete(lval *v) { free(v); }

lval *lval_exit_copy(lval *s, lval *d) {
  d->exit_code = s->exit_code;
  return d;
}

void lval_exit_print(lval *v) { printf("exit (%li)", v->exit_code); }
