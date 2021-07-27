#include "lval/lval.h"
#include "../builtin.h"
#include "../lenv.h"
#include "../ltypes.h"
#include <stdio.h>

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

lval *lval_read_string(mpc_ast_t *t) {
  t->contents[strlen(t->contents) - 1] = '\0';
  char *unescaped_string = malloc(strlen(t->contents + 1) + 1);
  strcpy(unescaped_string, t->contents + 1);
  unescaped_string = mpcf_unescape(unescaped_string);
  lval *string = lval_string(unescaped_string);
  free(unescaped_string);
  return string;
}

lval *lval_read(mpc_ast_t *t) {
  if (strstr(t->tag, "number")) {
    return lval_read_number(t);
  }
  if (strstr(t->tag, "symbol")) {
    return lval_symbol(t->contents);
  }
  if (strstr(t->tag, "string")) {
    return lval_read_string(t);
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
    if (strstr(t->children[i]->tag, "comment")) {
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

  lval *f = lval_pop(v, 0);
  if (f->type != LVAL_FUNCTION) {
    lval_delete(f);
    lval_delete(v);
    return lval_error(
        "S-Expression starts with incorrect type! Got %s, Expected %s.",
        ltype_name(f->type), ltype_name(LVAL_FUNCTION));
  }

  lval *result = lval_call(e, f, v);
  lval_delete(f);
  return result;
}

lval *lval_call(lenv *e, lval *f, lval *a) {
  if (f->builtin) {
    return f->builtin(e, a);
  }

  int given = a->count;
  int total = f->formals->count;

  while (a->count) {
    if (f->formals->count == 0) {
      lval_delete(a);
      return lval_error("Function passed too many arguments. "
                        "Got %i, Expected %i.",
                        given, total);
    }

    lval *symbol = lval_pop(f->formals, 0);

    if (strcmp(symbol->symbol, "&") == 0) {
      if (f->formals->count != 1) {
        lval_delete(a);
        return lval_error("Function format invalid. "
                          "Symbol '&' not follow by single symbol.");
      }

      lval *nsymbols = lval_pop(f->formals, 0);
      lenv_put(f->environment, nsymbols, builtin_list(e, a));
      lval_delete(symbol);
      lval_delete(nsymbols);
      break;
    }

    lval *value = lval_pop(a, 0);
    lenv_put(f->environment, symbol, value);
    lval_delete(symbol);
    lval_delete(value);
  }

  lval_delete(a);

  if (f->formals->count > 0 && strcmp(f->formals->cell[0]->symbol, "&") == 0) {
    if (f->formals->count != 2) {
      return lval_error("Function format invalid. "
                        "Symbol '&' not follow by single symbol.");
    }
    lval_delete(lval_pop(f->formals, 0));

    lval *symbol = lval_pop(f->formals, 0);
    lval *value = lval_qexpression();

    lenv_put(f->environment, symbol, value);
    lval_delete(symbol);
    lval_delete(value);
  }

  if (f->formals->count == 0) {
    f->environment->parent = e;
    return builtin_eval(f->environment,
                        lval_add(lval_sexpression(), lval_copy(f->body)));
  } else {
    return lval_copy(f);
  }
}

lval *lval_pop(lval *v, int index) {
  if (v->count <= 0) {
    return NULL;
  }

  lval *result = v->cell[index];
  lval **destination = &v->cell[index];
  lval **source = &v->cell[index + 1];
  int cells_to_move = v->count - index - 1;

  memmove(destination, source, sizeof(lval *) * cells_to_move);
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

int lval_equal(lval *x, lval *y) {

  if ((x->type & LVAL_NUMBER) && (y->type & LVAL_NUMBER)) {
    return lval_equal_number(x, y);
  }

  if (x->type != y->type) {
    return 0;
  }

  switch (x->type) {
  case LVAL_INTEGER:
  case LVAL_DECIMAL:
    return 0;

  case LVAL_OK:
    return 1;

  case LVAL_EXIT:
    return x->exit_code == y->exit_code;

  case LVAL_FILE:
    return x->file == y->file && (strcmp(x->file_name, y->file_name) == 0) &&
           (strcmp(x->mode, y->mode) == 0);
  case LVAL_ERROR:
    return (strcmp(x->error, y->error) == 0);
  case LVAL_SYMBOL:
    return (strcmp(x->symbol, y->symbol) == 0);
  case LVAL_STRING:
    return (strcmp(x->string, y->string) == 0);
  case LVAL_FUNCTION:
    if (x->builtin || y->builtin) {
      return x->builtin == y->builtin;
    } else {
      return lval_equal(x->formals, y->formals) && lval_equal(x->body, y->body);
    }
  case LVAL_QEXPRESSION:
  case LVAL_SEXPRESSION:
    if (x->count != y->count) {
      return 0;
    }
    for (int i = 0; i < x->count; i++) {
      if (!lval_equal(x->cell[i], y->cell[i])) {
        return 0;
      }
    }
    return 1;
  }
  return 0;
}

int lval_equal_number(lval *x, lval *y) {
  lval *xc = lval_copy(x);
  lval *yc = lval_copy(y);

  xc = builtin_convert_to_decimal_if_required(xc, yc);
  yc = builtin_convert_to_decimal_if_required(yc, xc);

  int result = 0;
  if (xc->type == LVAL_INTEGER) {
    result = xc->integer == yc->integer;
  }
  if (xc->type == LVAL_DECIMAL) {
    result = xc->decimal == yc->decimal;
  }

  lval_delete(xc);
  lval_delete(yc);

  return result;
}

lval *lval_add(lval *t, lval *v) {
  t->count++;
  t->cell = realloc(t->cell, sizeof(lval *) * t->count);
  t->cell[t->count - 1] = v;
  return t;
}

void lval_delete(lval *v) { v->delete (v); }

lval *lval_copy(lval *s) {
  lval *d = malloc(sizeof(lval));
  d->type = s->type;
  d->delete = s->delete;
  d->copy = s->copy;
  d->print = s->print;
  d->show = s->show;
  return s->copy(s, d);
}
void lval_print(lval *v) { v->print(v); }

void lval_println(lval *v) {
  lval_print(v);
  putchar('\n');
}

void lval_show(lval *v) { v->show(v); }
