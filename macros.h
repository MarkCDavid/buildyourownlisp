#ifndef macros_h
#define macros_h

#define LASSERT(arguments, condition, error) \
  if (!(condition)) { \
    lval_delete(arguments); \
    return lval_error(error); \
  }

#define LASSERT_ARG_COUNT(arguments, number, error) \
  LASSERT(arguments, arguments->count == number, error);

#define LASSERT_TYPE(arguments, index, expected, error) \
  LASSERT(arguments, arguments->cell[index]->type == expected, error);

#define LASSERT_NOT_EMPTY(arguments, index, error) \
  LASSERT(arguments, arguments->cell[index]->count != 0, error);

#endif
