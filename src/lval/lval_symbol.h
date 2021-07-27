#ifndef lval_symbol_h
#define lval_symbol_h

#include "../ltypes.h"

void lval_symbol_delete(lval *v);
lval *lval_symbol_copy(lval *s, lval *d);

#endif