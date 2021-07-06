#ifndef macros_h
#define macros_h

#define LASSERT(arguments, condition, format, ...) \
  if (!(condition)) { \
    lval* error = lval_error(format, ##__VA_ARGS__); \
    lval_delete(arguments); \
    return error; \
  }

#define LASSERT_ARG_COUNT(arguments, number, format, ...) \
  LASSERT(arguments, arguments->count == number, format, ##__VA_ARGS__);

#define LASSERT_ARG_COUNT_AT_LEAST(arguments, number, format, ...) \
  LASSERT(arguments, arguments->count >= number, format, ##__VA_ARGS__);

#define LASSERT_TYPE(arguments, index, expected, format, ...) \
  LASSERT(arguments, arguments->cell[index]->type & expected, format, ##__VA_ARGS__);

#define LASSERT_NOT_EMPTY(arguments, index, format, ...) \
  LASSERT(arguments, arguments->cell[index]->count != 0, format, ##__VA_ARGS__);

#endif
