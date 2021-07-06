#ifndef ltypes_h
#define ltypes_h

struct lval;
typedef struct lval lval;

struct lenv;
typedef struct lenv lenv;

typedef lval*(*lbuiltin)(lenv *e, lval *v);

struct lval {
  int type;
  int count;

  union {
    long integer;
    double decimal;
    struct lval **cell;
    lbuiltin builtin;
  };

  union 
  {
    char *error;
    char *symbol;
    char *builtin_name;
  };
  
};

struct lenv {
  int count;
  char** symbols;
  lval** values;
};

enum {
  LVAL_INTEGER = 1 << 0,
  LVAL_DECIMAL = 1 << 1,
  LVAL_SYMBOL = 1 << 2,
  LVAL_FUNCTION = 1 << 3,
  LVAL_ERROR = 1 << 4,
  LVAL_SEXPRESSION = 1 << 5,
  LVAL_QEXPRESSION = 1 << 6,

  LVAL_NUMBER = LVAL_INTEGER | LVAL_DECIMAL,
};

char* ltype_name(int type);

#endif