#include "ltypes.h"

char *ltype_name(int type) {
  switch (type) {
  case LVAL_INTEGER:
    return "Integer";
  case LVAL_DECIMAL:
    return "Decimal";
  case LVAL_SYMBOL:
    return "Symbol";
  case LVAL_STRING:
    return "String";
  case LVAL_FUNCTION:
    return "Function";
  case LVAL_FILE:
    return "File";
  case LVAL_ERROR:
    return "Error";
  case LVAL_SEXPRESSION:
    return "S-Expression";
  case LVAL_QEXPRESSION:
    return "Q-Expression";
  case LVAL_OK:
    return "OK";
  case LVAL_EXIT:
    return "Exit";
  case LVAL_NUMBER:
    return "Number";
  case LVAL_LIST:
    return "List";
  default:
    return "Unknown";
  }
}