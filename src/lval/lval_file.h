#ifndef lval_file_h
#define lval_file_h

#include "../ltypes.h"

void lval_file_delete(lval *v);
lval *lval_file_copy(lval *s, lval *d);
void lval_file_print(lval *v);
lval *lval_file_eval(lenv *e, lval *v);

#endif