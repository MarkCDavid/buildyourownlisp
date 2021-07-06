#include "lval.h"
#include "lenv.h"
#include "ltypes.h"
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

lval *lval_function(lbuiltin function, char* name) {
  lval *v = malloc(sizeof(lval));
  v->type = LVAL_FUNCTION;
  v->builtin = function;
  v->builtin_name = malloc(strlen(name) + 1);
  strcpy(v->builtin_name, name);
  return v;
}

lval *lval_error(char *format, ...) {
  lval *v = malloc(sizeof(lval));
  v->type = LVAL_ERROR;

  va_list va;
  va_start(va, format);

  v->error = malloc(512);
  vsnprintf(v->error, 511, format, va);
  v->error = realloc(v->error, strlen(v->error) + 1);

  va_end(va);
  return v;
}

lval *lval_sexpression(void) {
  lval *v = malloc(sizeof(lval));
  v->type = LVAL_SEXPRESSION;
  v->cell = NULL;
  v->count = 0;
  return v;
}

lval *lval_qexpression(void) {
  lval *v = malloc(sizeof(lval));
  v->type = LVAL_QEXPRESSION;
  v->cell = NULL;
  v->count = 0;
  return v;
}

lval *lval_read_number(mpc_ast_t *t) {
  if (strstr(t->tag, "decimal")) {
    errno = 0;
    double x = strtod(t->contents, NULL);
    return errno != ERANGE
               ? lval_decimal(x)
               : lval_error("Invalid number format '%s'.", t->contents);
  }

  if (strstr(t->tag, "integer")) {
    errno = 0;
    long x = strtol(t->contents, NULL, 10);
    return errno != ERANGE
               ? lval_integer(x)
               : lval_error("Invalid number format '%s'.", t->contents);
  }

  return lval_error("'%s' is not an integer or decimal", t->contents);
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
  if (strstr(t->tag, "sexpression")) {
    result = lval_sexpression();
  }
  if (strstr(t->tag, "qexpression")) {
    result = lval_qexpression();
  }

  for (int i = 0; i < t->children_num; i++) {
    if (strcmp(t->children[i]->contents, "(") == 0) {
      continue;
    }
    if (strcmp(t->children[i]->contents, ")") == 0) {
      continue;
    }
    if (strcmp(t->children[i]->contents, "{") == 0) {
      continue;
    }
    if (strcmp(t->children[i]->contents, "}") == 0) {
      continue;
    }
    if (strcmp(t->children[i]->tag, "regex") == 0) {
      continue;
    }
    result = lval_add(result, lval_read(t->children[i]));
  }

  return result;
}

lval *lval_eval(lenv *e, lval *v) {
  if (v->type == LVAL_SYMBOL) {
    lval *r = lenv_get(e, v);
    lval_delete(v);
    return r;
  }
  if (v->type == LVAL_SEXPRESSION) {
    return lval_eval_sexpression(e, v);
  }
  return v;
}

lval *lval_eval_sexpression(lenv *e, lval *v) {
  for (int i = 0; i < v->count; i++) {
    v->cell[i] = lval_eval(e, v->cell[i]);
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
  if (s->type != LVAL_FUNCTION) {
    lval_delete(s);
    lval_delete(v);
    return lval_error(
        "S-Expression starts with incorrect type! Got %s, Expected %s.",
        ltype_name(s->type), ltype_name(LVAL_FUNCTION));
  }

  lval *result = s->builtin(e, v);
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

lval *lval_join(lval *t, lval *v) {
  while (v->count) {
    t = lval_add(t, lval_pop(v, 0));
  }

  lval_delete(v);
  return t;
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
  case LVAL_FUNCTION:
    break;

  case LVAL_ERROR:
    free(v->error);
    break;

  case LVAL_SYMBOL:
    free(v->symbol);
    break;

  case LVAL_SEXPRESSION:
  case LVAL_QEXPRESSION:
    for (int i = 0; i < v->count; i++) {
      lval_delete(v->cell[i]);
    }
    free(v->cell);
  }

  free(v);
}

lval *lval_copy(lval *v) {

  lval *r = malloc(sizeof(lval));
  r->type = v->type;

  switch (v->type) {
  case LVAL_FUNCTION:
    r->builtin = v->builtin;
    r->builtin_name = malloc(strlen(v->builtin_name) + 1);
    strcpy(r->builtin_name, v->builtin_name);
    break;
  case LVAL_DECIMAL:
    r->decimal = v->decimal;
    break;
  case LVAL_INTEGER:
    r->integer = v->integer;
    break;

  case LVAL_ERROR:
    r->error = malloc(strlen(v->error) + 1);
    strcpy(r->error, v->error);
    break;

  case LVAL_SYMBOL:
    r->symbol = malloc(strlen(v->symbol) + 1);
    strcpy(r->symbol, v->symbol);
    break;

  case LVAL_SEXPRESSION:
  case LVAL_QEXPRESSION:
    r->count = v->count;
    r->cell = malloc(sizeof(lval *) * r->count);
    for (int i = 0; i < r->count; i++) {
      r->cell[i] = lval_copy(v->cell[i]);
    }
    break;
  }

  return r;
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
  case LVAL_FUNCTION:
    printf("<function '%s'>", v->builtin_name);
    break;
  case LVAL_ERROR:
    printf("%s", v->error);
    break;
  case LVAL_SEXPRESSION:
    lval_expression_print(v, '(', ')');
    break;
  case LVAL_QEXPRESSION:
    lval_expression_print(v, '{', '}');
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
