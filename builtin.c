#include "builtin.h"
#include "grammar.h"
#include "lenv.h"
#include "ltypes.h"
#include "lval.h"
#include "macros.h"

lval *builtin_add(lenv *e, lval *v) {
  LASSERT_ARG_COUNT_AT_LEAST("+", v, 1);

  for (int i = 0; i < v->count; i++) {
    LASSERT_TYPE("+", v, i, LVAL_NUMBER);
  }

  lval *x = lval_pop(v, 0);

  while (v->count) {
    lval *y = lval_pop(v, 0);

    x = builtin_convert_to_decimal_if_required(x, y);
    y = builtin_convert_to_decimal_if_required(y, x);

    if (x->type == LVAL_INTEGER) {
      x->integer += y->integer;
    }

    if (x->type == LVAL_DECIMAL) {
      x->decimal += y->decimal;
    }
    lval_delete(y);
  }
  lval_delete(v);
  return x;
}

lval *builtin_minus(lenv *e, lval *v) {
  LASSERT_ARG_COUNT_AT_LEAST("-", v, 1);

  if (v->count == 1) {
    return builtin_negate(e, v);
  }

  return builtin_subtract(e, v);
}

lval *builtin_multiply(lenv *e, lval *v) {
  LASSERT_ARG_COUNT_AT_LEAST("*", v, 1);

  for (int i = 0; i < v->count; i++) {
    LASSERT_TYPE("*", v, i, LVAL_NUMBER);
  }

  lval *x = lval_pop(v, 0);

  while (v->count) {
    lval *y = lval_pop(v, 0);

    x = builtin_convert_to_decimal_if_required(x, y);
    y = builtin_convert_to_decimal_if_required(y, x);

    if (x->type == LVAL_INTEGER) {
      x->integer *= y->integer;
    }

    if (x->type == LVAL_DECIMAL) {
      x->decimal *= y->decimal;
    }
    lval_delete(y);
  }
  lval_delete(v);
  return x;
}

lval *builtin_divide(lenv *e, lval *v) {
  LASSERT_ARG_COUNT_AT_LEAST("/", v, 1);

  for (int i = 0; i < v->count; i++) {
    LASSERT_TYPE("/", v, i, LVAL_NUMBER);
  }

  lval *x = lval_pop(v, 0);

  while (v->count) {
    lval *y = lval_pop(v, 0);

    x = builtin_convert_to_decimal_if_required(x, y);
    y = builtin_convert_to_decimal_if_required(y, x);

    if ((y->type == LVAL_INTEGER && y->integer == 0) ||
        (y->type == LVAL_DECIMAL && y->decimal == 0)) {
      lval_delete(x);
      lval_delete(y);
      lval_delete(v);
      return lval_error("Division by zero!");
    }

    if (x->type == LVAL_INTEGER) {
      x->integer /= y->integer;
    }

    if (x->type == LVAL_DECIMAL) {
      x->decimal /= y->decimal;
    }
    lval_delete(y);
  }
  lval_delete(v);
  return x;
}

lval *builtin_modulo(lenv *e, lval *v) {
  LASSERT_ARG_COUNT_AT_LEAST("%", v, 1);

  for (int i = 0; i < v->count; i++) {
    LASSERT_TYPE("%", v, i, LVAL_NUMBER);
  }

  lval *x = lval_pop(v, 0);

  if (x->type == LVAL_DECIMAL) {
    lval_delete(x);
    lval_delete(v);
    return lval_error("Modulo operation on decimal numbers!");
  }

  while (v->count) {
    lval *y = lval_pop(v, 0);

    if (y->type == LVAL_DECIMAL) {
      lval_delete(x);
      lval_delete(y);
      lval_delete(v);
      return lval_error("Modulo operation on decimal numbers!");
    }

    if (x->type == LVAL_INTEGER) {
      x->integer %= y->integer;
    }

    if (x->type == LVAL_DECIMAL) {
      x->decimal += y->decimal;
    }
    lval_delete(y);
  }
  lval_delete(v);
  return x;
}

lval *builtin_negate(lenv *e, lval *v) {
  LASSERT_ARG_COUNT("negate", v, 1);
  LASSERT_TYPE("negate", v, 0, LVAL_NUMBER);

  lval *x = lval_pop(v, 0);
  if (x->type == LVAL_INTEGER) {
    x->integer = -x->integer;
  }

  if (x->type == LVAL_DECIMAL) {
    x->decimal = -x->decimal;
  }
  lval_delete(v);
  return x;
}

lval *builtin_subtract(lenv *e, lval *v) {
  LASSERT_ARG_COUNT_AT_LEAST("-", v, 1);

  for (int i = 0; i < v->count; i++) {
    LASSERT_TYPE("-", v, i, LVAL_NUMBER);
  }

  lval *x = lval_pop(v, 0);

  while (v->count) {
    lval *y = lval_pop(v, 0);

    x = builtin_convert_to_decimal_if_required(x, y);
    y = builtin_convert_to_decimal_if_required(y, x);

    if (x->type == LVAL_INTEGER) {
      x->integer -= y->integer;
    }

    if (x->type == LVAL_DECIMAL) {
      x->decimal -= y->decimal;
    }
    lval_delete(y);
  }
  lval_delete(v);
  return x;
}

lval *builtin_list(lenv *e, lval *v) {
  v->type = LVAL_QEXPRESSION;
  return v;
}

lval *builtin_head(lenv *e, lval *v) {
  LASSERT_ARG_COUNT("head", v, 1);
  LASSERT_TYPE("head", v, 0, LVAL_LIST);

  if(v->cell[0]->type == LVAL_QEXPRESSION) {
    LASSERT_NOT_EMPTY("head", v, 0);
    lval *r = lval_take(v, 0);
    while (r->count > 1) {
      lval_delete(lval_pop(r, 1));
    }
    return r;
  } 

  if(v->cell[0]->type == LVAL_STRING) {
    LASSERT(v, strlen(v->cell[0]->string) != 0, "Function 'head' passed \"\"!");
    
    char* result_string = malloc(2);
    strncpy(result_string, v->cell[0]->string, 1);
    result_string[1] = '\0';

    lval* result = lval_string(result_string);
    
    free(result_string);
    lval_delete(v);
    return result;
  } 

  LERROR(v, "Function 'head' passed arguments of a new, unhandled type '%s'!", ltype_name(v->cell[0]->type));
}

lval *builtin_tail(lenv *e, lval *v) {
  LASSERT_ARG_COUNT("tail", v, 1);
  LASSERT_TYPE("tail", v, 0, LVAL_LIST);
   if(v->cell[0]->type == LVAL_QEXPRESSION) {
    LASSERT_NOT_EMPTY("tail", v, 0);

    lval *r = lval_take(v, 0);
    lval_delete(lval_pop(r, 0));
    return r;
   }

  if(v->cell[0]->type == LVAL_STRING) {
    LASSERT(v, strlen(v->cell[0]->string) != 0, "Function 'tail' passed \"\"!");
    
    char* result_string = malloc(strlen(v->cell[0]->string));
    strcpy(result_string, v->cell[0]->string + 1);

    lval* result = lval_string(result_string);
    
    free(result_string);
    lval_delete(v);
    return result;
  } 

  LERROR(v, "Function 'tail' passed arguments of a new, unhandled type '%s'!", ltype_name(v->cell[0]->type));
}

lval *builtin_eval(lenv *e, lval *v) {
  LASSERT_ARG_COUNT("eval", v, 1);
  LASSERT_TYPE("eval", v, 0, LVAL_QEXPRESSION);

  lval *r = lval_take(v, 0);
  r->type = LVAL_SEXPRESSION;
  return lval_eval(e, r);
}

lval *builtin_join(lenv *e, lval *v) {
  LASSERT_ARG_COUNT_AT_LEAST("join", v, 2);
  for (int i = 0; i < v->count; i++) {
    LASSERT_TYPE("join", v, i, LVAL_LIST);
  }

  if (v->cell[0]->type == LVAL_QEXPRESSION) {
    for (int i = 0; i < v->count; i++) {
      LASSERT_TYPE("join", v, i, LVAL_QEXPRESSION);
    }

    lval *r = lval_pop(v, 0);
    while (v->count) {
      r = lval_join(r, lval_pop(v, 0));
    }

    lval_delete(v);
    return r;
  }

  if (v->cell[0]->type == LVAL_STRING) {
    for (int i = 0; i < v->count; i++) {
      LASSERT_TYPE("join", v, i, LVAL_STRING);
    }

    int size = 1;
    int copied = 0;
    
    for(int i = 0; i < v->count; i++) {
      size += strlen(v->cell[i]->string);
    }
    
    char* joined_string = malloc(size);
    for(int i = 0; i < v->count; i++) {
      strcpy(joined_string + copied, v->cell[i]->string);
      copied += strlen(v->cell[i]->string);
    }

    joined_string[copied] = '\0';

    lval* result = lval_string(joined_string);
    
    free(joined_string);
    lval_delete(v);
    return result;
  }

  LERROR(v, "Function 'join' passed arguments of a new, unhandled type '%s'!", ltype_name(v->cell[0]->type));
}

lval *builtin_cons(lenv *e, lval *v) {
  LASSERT_ARG_COUNT("cons", v, 2);
  LASSERT_TYPE("cons", v, 1, LVAL_QEXPRESSION);

  lval *x = lval_pop(v, 0);
  lval *t = lval_pop(v, 0);

  t->count++;
  t->cell = realloc(t->cell, sizeof(lval *) * t->count);
  memmove(&t->cell[1], &t->cell[0], sizeof(lval *) * (t->count - 1));
  t->cell[0] = x;

  lval_delete(v);
  return t;
}

lval *builtin_len(lenv *e, lval *v) {
  LASSERT_ARG_COUNT("len", v, 1);
  LASSERT_TYPE("len", v, 0, LVAL_LIST);

  if(v->cell[0]->type == LVAL_QEXPRESSION) {
    lval *r = lval_integer(v->cell[0]->count);
    lval_delete(v);
    return r;
  } 

  if(v->cell[0]->type == LVAL_STRING) {
    lval *r = lval_integer(strlen(v->cell[0]->string));
    lval_delete(v);
    return r;
  }

  LERROR(v, "Function 'len' passed arguments of a new, unhandled type '%s'!", ltype_name(v->cell[0]->type));
}

lval *builtin_init(lenv *e, lval *v) {
  LASSERT_ARG_COUNT("init", v, 1);
  LASSERT_TYPE("init", v, 0, LVAL_LIST);

  if(v->cell[0]->type == LVAL_QEXPRESSION) {
    LASSERT_NOT_EMPTY("init", v, 0);
    lval *t = lval_pop(v, 0);

    lval_delete(lval_pop(t, t->count - 1));
    lval_delete(v);

    return t;
  } 

  if(v->cell[0]->type == LVAL_STRING) {
    LASSERT(v, strlen(v->cell[0]->string) != 0, "Function 'tail' passed \"\"!");
    int result_string_length = strlen(v->cell[0]->string);
    char* result_string = malloc(result_string_length);
    strncpy(result_string, v->cell[0]->string, result_string_length - 1);
    result_string[result_string_length - 1] = '\0';

    lval* result = lval_string(result_string);
    free(result_string);
    lval_delete(v);
    return result;
  }

  LERROR(v, "Function 'init' passed arguments of a new, unhandled type '%s'!", ltype_name(v->cell[0]->type));
}

lval *builtin_lambda(lenv *e, lval *v) {
  LASSERT_ARG_COUNT("\\", v, 2);
  LASSERT_TYPE("\\", v, 0, LVAL_QEXPRESSION);
  LASSERT_TYPE("\\", v, 1, LVAL_QEXPRESSION);

  for (int i = 0; i < v->cell[0]->count; i++) {
    LASSERT(v, (v->cell[0]->cell[i]->type == LVAL_SYMBOL),
            "Cannot define non-symbol. Got %s, Expected %s.",
            ltype_name(v->cell[0]->cell[i]->type), ltype_name(LVAL_SYMBOL));
  }

  lval *formals = lval_pop(v, 0);
  lval *body = lval_pop(v, 0);
  lval_delete(v);

  return lval_lambda(formals, body);
}

lval *builtin_read(lenv* e, lval *v) {
  LASSERT_ARG_COUNT("read", v, 1);
  LASSERT_TYPE("read", v, 0, LVAL_STRING);

  mpc_result_t result;
  if (mpc_parse("<string>", v->cell[0]->string, lispy_core, &result)) {
    lval *expression = lval_read(result.output);
    mpc_ast_delete(result.output);

    lval *result = lval_qexpression();

    while (expression->count) {
      result = lval_add(result, lval_pop(expression, 0));
    }

    lval_delete(expression);
    lval_delete(v);

    return result;
  } else {
    char *error_message = mpc_err_string(result.error);
    mpc_err_delete(result.error);

    lval *error = lval_error("Could not read: %s", error_message);
    free(error_message);
    lval_delete(v);
    return error;
  }
}

lval *builtin_print(lenv *e, lval *v) {
  for (int i = 0; i < v->count; i++) {
    lval_print(v->cell[i]);
    putchar(' ');
  }
  putchar('\n');
  lval_delete(v);

  return lval_ok();
}

lval *builtin_show(lenv* e, lval *v) {
  for (int i = 0; i < v->count; i++) {
    LASSERT_TYPE("show", v, i, LVAL_STRING);
  }

  for (int i = 0; i < v->count; i++) {
    lval_string_show(v->cell[i]);
    putchar(' ');
  }

  putchar('\n');
  lval_delete(v);

  return lval_ok();
}

lval *builtin_error(lenv *e, lval *v) {
  LASSERT_ARG_COUNT("error", v, 1);
  LASSERT_TYPE("error", v, 0, LVAL_STRING);

  lval *error = lval_error(v->cell[0]->string);

  lval_delete(v);
  return error;
}

lval *builtin_exit(lenv* e, lval *v) {
  LASSERT_ARG_COUNT("exit", v, 1);
  LASSERT_TYPE("exit", v, 0, LVAL_INTEGER);

  long exit_code = v->cell[0]->integer;
  lval_delete(v);
  return lval_exit(exit_code);
}

lval *builtin_file_open(lenv* e, lval *v) {
  LASSERT_ARG_COUNT("fopen", v, 2);
  LASSERT_TYPE("fopen", v, 0, LVAL_STRING);
  LASSERT_TYPE("fopen", v, 1, LVAL_STRING);

  FILE *file_descriptor = fopen(v->cell[0]->string, v->cell[1]->string);

  if(!file_descriptor) {
    lval* error = lval_error("Could not open file '%s' with mode '%s'.", v->cell[0]->string, v->cell[1]->string);
    lval_delete(v);
    return error;
  }
  
  lval* file = lval_file(file_descriptor, v->cell[0]->string, v->cell[1]->string);
  lval_delete(v);
  return file;
}

lval *builtin_file_close(lenv* e, lval *v) {
  LASSERT_ARG_COUNT("fclose", v, 1);
  LASSERT_TYPE("fclose", v, 0, LVAL_FILE);

  int success = fclose(v->cell[0]->file);

  if(!success) {
    lval* error = lval_error("Could not close file %s.", v->cell[0]->file_name);
    lval_delete(v);
    return error;
  }

  lval_delete(v);
  return lval_ok();
}
  

lval *builtin_file_read(lenv* e, lval *v) {
  LASSERT_ARG_COUNT("fread", v, 2);
  LASSERT_TYPE("fread", v, 0, LVAL_FILE);
  LASSERT_TYPE("fread", v, 1, LVAL_INTEGER);
  
  char* result_string = malloc(v->cell[1]->integer + 1);
  if(!fgets(result_string, v->cell[1]->integer + 1, v->cell[0]->file)) {
    lval* error = lval_error("Could not read %i symbols from file %s.", v->cell[1]->integer, v->cell[0]->file_name);
    lval_delete(v);
    return error;
  }

  lval* result = lval_string(result_string);
  free(result_string);
  lval_delete(v);
  
  return result;
}

lval *builtin_file_write(lenv* e, lval *v) {
  LASSERT_ARG_COUNT("fwrite", v, 2);
  LASSERT_TYPE("fwrite", v, 0, LVAL_FILE);
  LASSERT_TYPE("fwrite", v, 1, LVAL_STRING);

  if(fputs(v->cell[1]->string, v->cell[0]->file) < 0) {
    lval* error = lval_error("Could not write '%s' to file %s.", v->cell[1]->string, v->cell[0]->file_name);
    lval_delete(v);
    return error;
  }

  return lval_ok();
}

lval *builtin_greater_than(lenv *e, lval *v) {
  return builtin_ordinal(e, v, ">");
}

lval *builtin_less_than(lenv *e, lval *v) { return builtin_ordinal(e, v, "<"); }

lval *builtin_greater_than_or_equals(lenv *e, lval *v) {
  return builtin_ordinal(e, v, ">=");
}

lval *builtin_less_than_or_equals(lenv *e, lval *v) {
  return builtin_ordinal(e, v, "<=");
}

lval *builtin_ordinal(lenv *e, lval *v, char *function) {
  LASSERT_ARG_COUNT(function, v, 2);
  LASSERT_TYPE(function, v, 0, LVAL_NUMBER);
  LASSERT_TYPE(function, v, 1, LVAL_NUMBER);

  lval *x = lval_pop(v, 0);
  lval *y = lval_pop(v, 0);

  x = builtin_convert_to_decimal_if_required(x, y);
  y = builtin_convert_to_decimal_if_required(y, x);

  int result;
  if (strcmp(function, ">") == 0) {
    if (x->type == LVAL_INTEGER) {
      result = (x->integer > y->integer);
    }
    if (x->type == LVAL_DECIMAL) {
      result = (x->decimal > y->decimal);
    }
  }
  if (strcmp(function, "<") == 0) {
    if (x->type == LVAL_INTEGER) {
      result = (x->integer < y->integer);
    }
    if (x->type == LVAL_DECIMAL) {
      result = (x->decimal < y->decimal);
    }
  }
  if (strcmp(function, ">=") == 0) {
    if (x->type == LVAL_INTEGER) {
      result = (x->integer <= y->integer);
    }
    if (x->type == LVAL_DECIMAL) {
      result = (x->decimal <= y->decimal);
    }
  }
  if (strcmp(function, "<=") == 0) {
    if (x->type == LVAL_INTEGER) {
      result = (x->integer <= y->integer);
    }
    if (x->type == LVAL_DECIMAL) {
      result = (x->decimal <= y->decimal);
    }
  }

  lval_delete(x);
  lval_delete(y);
  lval_delete(v);
  return lval_integer(result);
}

lval *builtin_equals(lenv *e, lval *v) {
  return builtin_comparison(e, v, "==");
}

lval *builtin_not_equals(lenv *e, lval *v) {
  return builtin_comparison(e, v, "!=");
}

lval *builtin_comparison(lenv *e, lval *v, char *operator) {
  LASSERT_ARG_COUNT(operator, v, 2);

  int result;
  if (strcmp(operator, "==") == 0) {
    result = lval_equal(v->cell[0], v->cell[1]);
  }
  if (strcmp(operator, "!=") == 0) {
    result = !lval_equal(v->cell[0], v->cell[1]);
  }
  lval_delete(v);
  return lval_integer(result);
}

lval *builtin_not(lenv *e, lval *v) {
  LASSERT_ARG_COUNT("!", v, 1);
  LASSERT_TYPE("!", v, 0, LVAL_INTEGER);

  lval *result = lval_pop(v, 0);
  result->integer = result->integer ? 0 : 1;

  lval_delete(v);
  return result;
}

lval *builtin_and(lenv *e, lval *v) {
  LASSERT_ARG_COUNT_AT_LEAST("&&", v, 2);
  for (int i = 0; i < v->count; i++) {
    LASSERT_TYPE("&&", v, i, LVAL_INTEGER);
  }

  lval *x = lval_pop(v, 0);

  while (v->count) {
    lval *y = lval_pop(v, 0);
    x->integer = x->integer && y->integer;
    lval_delete(y);
  }

  lval_delete(v);
  return x;
}

lval *builtin_or(lenv *e, lval *v) {
  LASSERT_ARG_COUNT_AT_LEAST("||", v, 2);
  for (int i = 0; i < v->count; i++) {
    LASSERT_TYPE("||", v, i, LVAL_INTEGER);
  }

  lval *x = lval_pop(v, 0);

  while (v->count) {
    lval *y = lval_pop(v, 0);
    x->integer = x->integer || y->integer;
    lval_delete(y);
  }

  lval_delete(v);
  return x;
}

lval *builtin_if(lenv *e, lval *v) {
  LASSERT_ARG_COUNT("if", v, 3);
  LASSERT_TYPE("if", v, 0, LVAL_INTEGER);
  LASSERT_TYPE("if", v, 1, LVAL_QEXPRESSION);
  LASSERT_TYPE("if", v, 2, LVAL_QEXPRESSION);

  v->cell[1]->type = LVAL_SEXPRESSION;
  v->cell[2]->type = LVAL_SEXPRESSION;

  lval *result;
  if (v->cell[0]->integer) {
    result = lval_eval(e, lval_pop(v, 1));
  } else {
    result = lval_eval(e, lval_pop(v, 2));
  }

  lval_delete(v);
  return result;
}

lval *builtin_def(lenv *e, lval *v) { return builtin_variable(e, v, "def"); }

lval *builtin_put(lenv *e, lval *v) { return builtin_variable(e, v, "="); }

lval *builtin_variable(lenv *e, lval *v, char *function) {
  LASSERT_TYPE(function, v, 0, LVAL_QEXPRESSION);

  lval *symbols = v->cell[0];

  for (int i = 0; i < symbols->count; i++) {
    LASSERT_TYPE(function, v->cell[0], i, LVAL_SYMBOL);
  }

  if (symbols->count != v->count - 1) {
    lval *error = lval_error(
        "Function '%s' cannot define incorrect number of values to symbols! "
        "Got %i, Expected %i.",
        function, symbols->count, v->count - 1);
    lval_delete(v);
    return error;
  }

  lval *builtin = lenv_has_builtins(e, symbols);
  if (builtin) {
    lval *error = lval_error("Cannot redefine builtin function '%s'.",
                             builtin->builtin_name);
    lval_delete(builtin);
    lval_delete(v);
    return error;
  }

  for (int i = 0; i < symbols->count; i++) {
    if (strcmp(function, "def") == 0) {
      lenv_def(e, symbols->cell[i], v->cell[i + 1]);
    }
    if (strcmp(function, "=") == 0) {
      lenv_put(e, symbols->cell[i], v->cell[i + 1]);
    }
  }

  lval_delete(v);
  return lval_ok();
}

lval *builtin_fun(lenv *e, lval *v) {
  LASSERT_TYPE("fun", v, 0, LVAL_QEXPRESSION);
  LASSERT_TYPE("fun", v, 1, LVAL_QEXPRESSION);

  lval *arguments = lval_add(lval_qexpression(), lval_pop(v, 0));
  lval *arguments_head = builtin_head(e, lval_copy(arguments));
  lval *arguments_tail = builtin_tail(e, lval_copy(arguments));

  lval *body = lval_pop(v, 0);
  lval *lambda = lval_lambda(arguments_tail, body);

  lval *def_arguments = lval_qexpression();
  lval_add(def_arguments, arguments_head);
  lval_add(def_arguments, lambda);

  lval_delete(v);
  lval_delete(arguments);
  return builtin_def(e, def_arguments);
}

lval *builtin_convert_to_decimal_if_required(lval *t, lval *c) {
  if (t->type == LVAL_DECIMAL) {
    return t;
  }

  if (c->type == LVAL_DECIMAL) {
    lval *r = lval_decimal(t->integer);
    lval_delete(t);
    return r;
  }

  return t;
}

lval *builtin_load(lenv *e, lval *v) {
  LASSERT_ARG_COUNT("load", v, 1);
  LASSERT_TYPE("load", v, 0, LVAL_STRING);

  mpc_result_t result;
  if (mpc_parse_contents(v->cell[0]->string, lispy_core, &result)) {
    lval *expression = lval_read(result.output);
    mpc_ast_delete(result.output);

    while (expression->count) {
      lval *x = lval_eval(e, lval_pop(expression, 0));
      if (x->type == LVAL_ERROR) {
        lval_println(x);
      }
      lval_delete(x);
    }

    lval_delete(expression);
    lval_delete(v);

    return lval_ok();
  } else {
    char *error_message = mpc_err_string(result.error);
    mpc_err_delete(result.error);

    lval *error = lval_error("Could not load Library %s", error_message);
    free(error_message);
    lval_delete(v);
    return error;
  }
}