#ifndef lval_function_h
#define lval_function_h

#include "../ltypes.h"

void lval_function_delete(lval *v);
lval *lval_function_copy(lval *s, lval *d);
void lval_function_print(lval *v);
lval *lval_function_eval(lenv *e, lval *v);
lval *lval_function_call(lenv* e, lval *f, lval *a);

#endif