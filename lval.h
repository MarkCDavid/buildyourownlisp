#ifndef lval_h
#define lval_h

#include "ltypes.h"
#include "mpc.h"

lval *lval_integer(long integer);
lval *lval_decimal(double decimal);
lval *lval_symbol(char *symbol);
lval *lval_function(lbuiltin function, char *name);
lval *lval_error(char *format, ...);
lval *lval_sexpression(void);
lval *lval_qexpression(void);
lval *lval_lambda(lval *formals, lval *body);

lval *lval_read_number(mpc_ast_t *t);
lval *lval_read(mpc_ast_t *t);

lval *lval_eval(lenv *e, lval *v);
lval *lval_eval_sexpression(lenv *e, lval *v);
lval *lval_call(lenv *e, lval *f, lval *a);

lval *lval_pop(lval *v, int index);
lval *lval_take(lval *v, int index);

lval *lval_add(lval *t, lval *v);
lval *lval_join(lval *t, lval *v);

void lval_delete(lval *v);
lval *lval_copy(lval *v);
void lval_print(lval *v);
void lval_println(lval *v);
void lval_expression_print(lval *v, char open, char close);
void lval_function_print(lval *v);
void lval_function_body_print(lenv *e, lval *v, char open, char close);

#endif