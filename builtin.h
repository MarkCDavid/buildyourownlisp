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
lval *builtin_lambda(lenv* e, lval *v);
lval *builtin_read(lenv* e, lval *v);
lval *builtin_print(lenv* e, lval *v);
lval *builtin_show(lenv* e, lval *v);
lval *builtin_error(lenv* e, lval *v);
lval *builtin_exit(lenv* e, lval *v);

lval *builtin_file_open(lenv* e, lval *v);
lval *builtin_file_close(lenv* e, lval *v);
lval *builtin_file_read(lenv* e, lval *v);
lval *builtin_file_write(lenv* e, lval *v);

lval *builtin_greater_than(lenv* e, lval *v);
lval *builtin_less_than(lenv* e, lval *v);
lval *builtin_greater_than_or_equals(lenv* e, lval *v);
lval *builtin_less_than_or_equals(lenv* e, lval *v);
lval *builtin_ordinal(lenv* e, lval *v, char* operator);

lval *builtin_equals(lenv* e, lval *v);
lval *builtin_not_equals(lenv* e, lval *v);

lval *builtin_not(lenv* e, lval *v);
lval *builtin_and(lenv* e, lval *v);
lval *builtin_or(lenv* e, lval *v);

lval *builtin_comparison(lenv* e, lval *v, char* operator);

lval *builtin_if(lenv* e, lval *v);

lval *builtin_def(lenv* e, lval *v);
lval *builtin_put(lenv* e, lval *v);
lval *builtin_variable(lenv* e, lval *v, char* function);
lval *builtin_fun(lenv* e, lval *v);

lval *builtin_convert_to_decimal_if_required(lval *t, lval *c);

lval *builtin_load(lenv* e, lval *v);

#endif