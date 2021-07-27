#ifndef lval_error_h
#define lval_error_h

#include "ltypes.h"

void lval_error_delete(lval *v);
lval *lval_error_copy(lval *s, lval *d);

#endif