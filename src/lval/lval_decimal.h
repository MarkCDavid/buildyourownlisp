#ifndef lval_decimal_h
#define lval_decimal_h

#include "../ltypes.h"

void lval_decimal_delete(lval *v);
lval *lval_decimal_copy(lval *s, lval *d);
void lval_decimal_print(lval *v);

#endif