#ifndef lval_integer_h
#define lval_integer_h

#include "../ltypes.h"

void lval_integer_delete(lval *v);
lval *lval_integer_copy(lval *s, lval *d);
void lval_integer_print(lval *v);
lval *lval_integer_eval(lenv* e, lval *v);

#endif