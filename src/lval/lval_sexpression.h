#ifndef lval_sexpression_h
#define lval_sexpression_h

#include "../ltypes.h"

void lval_sexpression_delete(lval *v);
lval *lval_sexpression_copy(lval *s, lval *d);
void lval_sexpression_print(lval *v);
lval *lval_sexpression_eval(lenv* e, lval *v);

#endif