struct lval;
typedef struct lval lval;

struct lval {
  int type;

  union {
    long integer;
    double decimal;
    int error;
  };
};

enum { LVAL_INT, LVAL_DEC, LVAL_ERR };
enum { LERR_DIV_ZERO, LERR_DEC_REMAINDER, LERR_BAD_OP, LERR_BAD_NUM };

lval lval_integer(long x);
lval lval_decimal(double x);
lval lval_error(int x);

void lval_print(lval v);
void lval_println(lval v);
