#ifndef macros_h
#define macros_h

#define LASSERT(arguments, condition, format, ...) \
  if (!(condition)) { \
    lval* error = lval_error(format, ##__VA_ARGS__); \
    lval_delete(arguments); \
    return error; \
  }

#define LASSERT_ARG_COUNT(function, arguments, number) \
  LASSERT(arguments, arguments->count == number, \
    "Function '%s' passed incorrect number of arguments! Got %s, Expected %s." \
    function, v->count, number);

#define LASSERT_ARG_COUNT_AT_LEAST(function, arguments, number) \
  LASSERT(arguments, arguments->count >= number, \
    "Function '%s' passed incorrect number of arguments! Got %s, Expected >=%s." \
    function, v->count, number);

#define LASSERT_TYPE(function, arguments, index, expected) \
  LASSERT(arguments, arguments->cell[index]->type & expected, \
    "Function '%s' passed incorrect type for argument %i. " \
    "Got %s, Expected %s.", function, index, ltype_name(v->cell[index]->type), ltype_name(LVAL_SYMBOL));

#define LASSERT_NOT_EMPTY(function, arguments, index) \
  LASSERT(arguments, arguments->cell[index]->count != 0, \
    "Function '%s' passed {}!", function);

#define LASSERT_DEF_COUNT(function, arguments, symbols) \
  LASSERT(arguments, symbols->count == arguments->count - 1, \
    "Function '%s'cannot define incorrect number of values to symbols! " \
    "Got %s, Expected %s.", function, symbols->count, arguments->count - 1);

#endif
