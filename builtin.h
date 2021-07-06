#ifndef builtin_h
#define builtin_h

#include "lval.h"

lval *builtin(lval *v, char *function);
lval *builtin_operator(lval *v, char *operator);

lval *builtin_negate(lval *x);
lval *builtin_add(lval *x, lval *y);
lval *builtin_subtract(lval *x, lval *y);
lval *builtin_multiply(lval *x, lval *y);
lval *builtin_divide(lval *x, lval *y);
lval *builtin_modulo(lval *x, lval *y);

lval *builtin_list(lval *x);
lval *builtin_head(lval *x);
lval *builtin_tail(lval *x);
lval *builtin_eval(lval *x);
lval *builtin_join(lval *x);
lval *builtin_cons(lval *x);
lval *builtin_len(lval *x);
lval *builtin_init(lval *x);

lval *builtin_convert_to_decimal(lval *v);

#endif