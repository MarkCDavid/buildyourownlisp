#ifndef lval_ok_h
#define lval_ok_h

#include "../ltypes.h"

void lval_ok_delete(lval *v);
lval *lval_ok_copy(lval *s, lval *d);
void lval_ok_print(lval *v);
lval *lval_ok_eval(lenv* e, lval *v);

#endif