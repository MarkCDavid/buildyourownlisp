#ifndef lval_sexpression_h
#define lval_sexpression_h

#include "ltypes.h"

void lval_sexpression_delete(lval *v);
lval *lval_sexpression_copy(lval *s, lval *d);

#endif