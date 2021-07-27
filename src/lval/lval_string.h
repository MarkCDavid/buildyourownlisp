#ifndef lval_string_h
#define lval_string_h

#include "ltypes.h"

void lval_string_delete(lval *v);
lval *lval_string_copy(lval *s, lval *d);

#endif