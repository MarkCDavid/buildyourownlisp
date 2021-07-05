#include "lval.h"
#include <stdio.h>

lval lval_integer(long x) {
  lval v;
  v.type = LVAL_INT;
  v.integer = x;
  return v;
}

lval lval_decimal(double x) {
  lval v;
  v.type = LVAL_DEC;
  v.decimal = x;
  return v;
}

lval lval_error(int x) {
  lval v;
  v.type = LVAL_ERR;
  v.error = x;
  return v;
}

void lval_print(lval v) {
  switch (v.type) {
  case LVAL_INT:
    printf("%li", v.integer);
    break;
  case LVAL_DEC:
    printf("%f", v.decimal);
    break;
  case LVAL_ERR:
    if (v.error == LERR_DIV_ZERO) {
      printf("Error: Division By Zero!");
    }
    if (v.error == LERR_BAD_OP) {
      printf("Error: Invalid Operator!");
    }
    if (v.error == LERR_BAD_NUM) {
      printf("Error: Invalid Number!");
    }
    if (v.error == LERR_DEC_MOD) {
      printf("Error: Remainder Operation on non Integer!");
    }
    break;
  }
}

void lval_println(lval v) {
  lval_print(v);
  putchar('\n');
}