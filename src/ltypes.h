#ifndef ltypes_h
#define ltypes_h

#include <stdio.h>

struct lval;
typedef struct lval lval;

struct lenv;
typedef struct lenv lenv;

typedef lval*(*lbuiltin)(lenv *e, lval *v);

typedef void(*lv_void_lval)(lval *v);
typedef lval*(*lv_lval_lval_lval)(lval *x, lval *y);

struct lval {
  int type;
  int count;

  lv_void_lval delete;
  lv_lval_lval_lval copy;

  union {
    long integer;
    long exit_code;
    double decimal;
    lbuiltin builtin;
    struct lval **cell;
    FILE *file;
  };

  union 
  {
    char *error;
    char *symbol;
    char *string;
    char *builtin_name;
    char *mode;
  };

  union {
    lenv* environment;
    char* file_name;
  };

  lval* formals;
  lval* body;
};

struct lenv {
  lenv* parent;
  int count;
  char** symbols;
  lval** values;
};

enum {
  LVAL_INTEGER = 1 << 0,
  LVAL_DECIMAL = 1 << 1,
  LVAL_SYMBOL = 1 << 2,
  LVAL_STRING = 1 << 3,
  LVAL_FUNCTION = 1 << 4,
  LVAL_ERROR = 1 << 5,
  LVAL_SEXPRESSION = 1 << 6,
  LVAL_QEXPRESSION = 1 << 7,
  LVAL_FILE = 1 << 8,
  LVAL_OK = 1 << 30,
  LVAL_EXIT = 1 << 31,

  LVAL_NUMBER = LVAL_INTEGER | LVAL_DECIMAL,
  LVAL_LIST = LVAL_QEXPRESSION | LVAL_STRING,
};

char* ltype_name(int type);

#endif