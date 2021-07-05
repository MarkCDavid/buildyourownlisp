#include "lval.h"
#include "builtin.h"
#include <stdio.h>

lval *lval_integer(long integer) {
  lval *v = malloc(sizeof(lval));
  v->type = LVAL_INTEGER;
  v->integer = integer;
  return v;
}

lval *lval_decimal(double decimal) {
  lval *v = malloc(sizeof(lval));
  v->type = LVAL_DECIMAL;
  v->decimal = decimal;
  return v;
}

lval *lval_symbol(char *symbol) {
  lval *v = malloc(sizeof(lval));
  v->type = LVAL_SYMBOL;
  v->symbol = malloc(strlen(symbol) + 1);
  strcpy(v->symbol, symbol);
  return v;
}

lval *lval_error(char *error) {
  lval *v = malloc(sizeof(lval));
  v->type = LVAL_ERROR;
  v->error = malloc(strlen(error) + 1);
  strcpy(v->error, error);
  return v;
}

lval *lval_sexpression(void) {
  lval *v = malloc(sizeof(lval));
  v->type = LVAL_SEXPRESSION;
  v->cell = NULL;
  v->count = 0;
  return v;
}

lval *lval_read_number(mpc_ast_t *t) {
  if (strstr(t->tag, "decimal")) {
    errno = 0;
    double x = strtod(t->contents, NULL);
    return errno != ERANGE ? lval_decimal(x) : lval_error("invalid number");
  }

  if (strstr(t->tag, "integer")) {
    errno = 0;
    long x = strtol(t->contents, NULL, 10);
    return errno != ERANGE ? lval_integer(x) : lval_error("invalid number");
  }

  return lval_error("not an integer/decimal");
}

lval *lval_read(mpc_ast_t *t) {
  if (strstr(t->tag, "number")) {
    return lval_read_number(t);
  }
  if (strstr(t->tag, "symbol")) {
    return lval_symbol(t->contents);
  }

  lval *result = NULL;
  if (strcmp(t->tag, ">") == 0) {
    result = lval_sexpression();
  }
  if (strstr(t->tag, "sexpr")) {
    result = lval_sexpression();
  }

  for (int i = 0; i < t->children_num; i++) {
    if (strcmp(t->children[i]->contents, "(") == 0) {
      continue;
    }
    if (strcmp(t->children[i]->contents, ")") == 0) {
      continue;
    }
    if (strcmp(t->children[i]->tag, "regex") == 0) {
      continue;
    }
    result = lval_add(result, lval_read(t->children[i]));
  }

  return result;
}

lval *lval_eval(lval *v) {
  if (v->type == LVAL_SEXPRESSION) {
    return lval_eval_sexpression(v);
  }
  return v;
}

lval *lval_eval_sexpression(lval *v) {
  for (int i = 0; i < v->count; i++) {
    v->cell[i] = lval_eval(v->cell[i]);
  }

  for (int i = 0; i < v->count; i++) {
    if (v->cell[i]->type == LVAL_ERROR) {
      return lval_take(v, i);
    }
  }

  if (v->count == 0) {
    return v;
  }
  if (v->count == 1) {
    return lval_take(v, 0);
  }

  lval *s = lval_pop(v, 0);
  if (s->type != LVAL_SYMBOL) {
    lval_delete(s);
    lval_delete(v);
    return lval_error("S-Expression does not start with a symbol!");
  }

  lval *result = builtin_operator(v, s->symbol);
  lval_delete(s);
  return result;
}

lval *lval_pop(lval *v, int index) {
  lval *result = v->cell[index];
  memmove(&v->cell[index], &v->cell[index + 1],
          sizeof(lval *) * (v->count - index - 1));
  v->count--;
  v->cell = realloc(v->cell, sizeof(lval *) * v->count);
  return result;
}

lval *lval_take(lval *v, int index) {
  lval *result = lval_pop(v, index);
  lval_delete(v);
  return result;
}

lval *lval_add(lval *t, lval *v) {
  t->count++;
  t->cell = realloc(t->cell, sizeof(lval *) * t->count);
  t->cell[t->count - 1] = v;
  return t;
}

void lval_delete(lval *v) {
  switch (v->type) {
  case LVAL_DECIMAL:
  case LVAL_INTEGER:
    break;

  case LVAL_ERROR:
    free(v->error);
    break;

  case LVAL_SYMBOL:
    free(v->symbol);
    break;

  case LVAL_SEXPRESSION:
    for (int i = 0; i < v->count; i++) {
      lval_delete(v->cell[i]);
    }
    free(v->cell);
  }

  free(v);
}

void lval_print(lval *v) {
  switch (v->type) {
  case LVAL_INTEGER:
    printf("%li", v->integer);
    break;
  case LVAL_DECIMAL:
    printf("%f", v->decimal);
    break;
  case LVAL_SYMBOL:
    printf("%s", v->symbol);
    break;
  case LVAL_ERROR:
    printf("%s", v->error);
    break;
  case LVAL_SEXPRESSION:
    lval_expression_print(v, '(', ')');
    break;
  }
}

void lval_println(lval *v) {
  lval_print(v);
  putchar('\n');
}

void lval_expression_print(lval *v, char open, char close) {
  putchar(open);
  for (int i = 0; i < v->count; i++) {
    lval_print(v->cell[i]);
    if (i != (v->count - 1)) {
      putchar(' ');
    }
  }
  putchar(close);
}
