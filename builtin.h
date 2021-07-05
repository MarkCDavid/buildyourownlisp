#ifndef builtin_h
#define builtin_h

#include "lval.h"

lval* builtin_operator(lval* v, char* operator);

lval* builtin_negate(lval* x);
lval* builtin_add(lval* x, lval* y);
lval* builtin_subtract(lval* x, lval* y);
lval* builtin_multiply(lval* x, lval* y);
lval* builtin_divide(lval* x, lval* y);
lval* builtin_modulo(lval* x, lval* y);

lval* builtin_convert_to_decimal(lval* v);

#endif