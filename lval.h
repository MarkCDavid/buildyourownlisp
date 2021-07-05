#ifndef lval_h
#define lval_h

#include "mpc.h"

struct lval;
typedef struct lval lval;

struct lval {
  int type;
  int count;

  union {
    long integer;
    double decimal;
    char *error;
    char *symbol;
    struct lval **cell;
  };
};

enum { LVAL_INTEGER, LVAL_DECIMAL, LVAL_SYMBOL, LVAL_ERROR, LVAL_SEXPRESSION };

lval *lval_integer(long integer);
lval *lval_decimal(double decimal);
lval *lval_symbol(char *error);
lval *lval_error(char *error);
lval *lval_sexpression(void);

lval *lval_read_number(mpc_ast_t *t);
lval *lval_read(mpc_ast_t *t);

lval *lval_eval(lval *v);
lval *lval_eval_sexpression(lval *v);

lval *lval_pop(lval *v, int index);
lval *lval_take(lval *v, int index);

lval *lval_add(lval *t, lval *v);

void lval_delete(lval *v);
void lval_print(lval *v);
void lval_println(lval *v);
void lval_expression_print(lval *v, char open, char close);

#endif