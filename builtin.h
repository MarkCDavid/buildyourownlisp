#ifndef builtin_h
#define builtin_h

#include "ltypes.h"

lval *builtin_add(lenv* e, lval *v);
lval *builtin_minus(lenv* e, lval *v);
lval *builtin_multiply(lenv* e, lval *v);
lval *builtin_divide(lenv* e, lval *v);
lval *builtin_modulo(lenv* e, lval *v);

lval *builtin_negate(lenv* e, lval *v);
lval *builtin_subtract(lenv* e, lval *v);

lval *builtin_list(lenv* e, lval *v);
lval *builtin_head(lenv* e, lval *v);
lval *builtin_tail(lenv* e, lval *v);
lval *builtin_eval(lenv* e, lval *v);
lval *builtin_join(lenv* e, lval *v);
lval *builtin_cons(lenv* e, lval *v);
lval *builtin_len(lenv* e, lval *v);
lval *builtin_init(lenv* e, lval *v);
lval *builtin_def(lenv* e, lval *v);

lval *builtin_convert_to_decimal_if_required(lval *t, lval *c);

#endif