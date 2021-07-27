#ifndef lval_ok_h
#define lval_ok_h

#include "../ltypes.h"

void lval_ok_delete(lval *v);
lval *lval_ok_copy(lval *s, lval *d);
void lval_ok_print(lval *v);

#endif