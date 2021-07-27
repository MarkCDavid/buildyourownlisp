#ifndef lval_exit_h
#define lval_exit_h

#include "../ltypes.h"

void lval_exit_delete(lval *v);
lval *lval_exit_copy(lval *s, lval *d);
void lval_exit_print(lval *v);
lval *lval_exit_eval(lenv *e, lval *v);

#endif