#ifndef lval_lambda_h
#define lval_lambda_h

#include "../ltypes.h"

void lval_lambda_delete(lval *v);
lval *lval_lambda_copy(lval *s, lval *d);
void lval_lambda_print(lval *v);
lval *lval_lambda_eval(lenv* e, lval *v);
lval *lval_lambda_call(lenv* e, lval *f, lval *a);

#endif