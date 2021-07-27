#ifndef lval_string_h
#define lval_string_h

#include "../ltypes.h"

void lval_string_delete(lval *v);
lval *lval_string_copy(lval *s, lval *d);
void lval_string_print(lval *v);
void lval_string_show(lval *v);
lval *lval_string_eval(lenv* e, lval *v);

#endif