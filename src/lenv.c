#include "lenv.h"
#include "lval/lval.h"
#include "builtin.h"
#include <stdlib.h>

lenv *lenv_new(void) {
  lenv *env = malloc(sizeof(lenv));
  env->parent = NULL;
  env->count = 0;
  env->symbols = NULL;
  env->values = NULL;
  return env;
}

lenv *lenv_copy(lenv *env) {
  lenv *r = malloc(sizeof(lenv));
  r->parent = env->parent;
  r->count = env->count;
  r->symbols = malloc(sizeof(char*) * r->count);
  r->values = malloc(sizeof(char*) * r->count);
  for(int i = 0; i < r->count; i++) {
    r->symbols[i] = malloc(strlen(env->symbols[i]) + 1);
    strcpy(r->symbols[i], env->symbols[i]);
    r->values[i] = lval_copy(env->values[i]);
  }
  return r;
}

void lenv_delete(lenv *env) {
  for (int i = 0; i < env->count; i++) {
    free(env->symbols[i]);
    lval_delete(env->values[i]);
  }

  free(env->symbols);
  free(env->values);
  free(env);
}

lval *lenv_get(lenv *env, lval *key) {
  for (int i = 0; i < env->count; i++) {
    if (strcmp(env->symbols[i], key->symbol) == 0) {
      return lval_copy(env->values[i]);
    }
  }

  if(env->parent) {
    return lenv_get(env->parent, key);
  } else {
    return lval_error("Unbound symbol '%s'!", key->symbol);
  }
}

void lenv_put(lenv *env, lval *key, lval *value) {
  for (int i = 0; i < env->count; i++) {
    if (strcmp(env->symbols[i], key->symbol) == 0) {
      lval_delete(env->values[i]);
      env->values[i] = lval_copy(value);
      return;
    }
  }

  env->count++;
  env->values = realloc(env->values, sizeof(lval *) * env->count);
  env->symbols = realloc(env->symbols, sizeof(char *) * env->count);

  env->values[env->count - 1] = lval_copy(value);
  env->symbols[env->count - 1] = malloc(strlen(key->symbol) + 1);
  strcpy(env->symbols[env->count - 1], key->symbol);
}

void lenv_def(lenv* env, lval* key, lval* value) {
  while(env->parent) {
    env = env->parent;
  }
  lenv_put(env, key, value);
}

void lenv_add_builtins(lenv *env) {
  lenv_add_builtin(env, "list", builtin_list);
  lenv_add_builtin(env, "head", builtin_head);
  lenv_add_builtin(env, "tail", builtin_tail);
  lenv_add_builtin(env, "last", builtin_last);
  lenv_add_builtin(env, "eval", builtin_eval);
  lenv_add_builtin(env, "join", builtin_join);
  lenv_add_builtin(env, "cons", builtin_cons);
  lenv_add_builtin(env, "len", builtin_len);
  lenv_add_builtin(env, "init", builtin_init);
  lenv_add_builtin(env, "\\", builtin_lambda);
  lenv_add_builtin(env, "load", builtin_load);
  lenv_add_builtin(env, "read", builtin_read);
  lenv_add_builtin(env, "print", builtin_print);
  lenv_add_builtin(env, "show", builtin_show);
  lenv_add_builtin(env, "error", builtin_error);
  lenv_add_builtin(env, "exit", builtin_exit);


  lenv_add_builtin(env, "fopen", builtin_file_open);
  lenv_add_builtin(env, "fclose", builtin_file_close);
  lenv_add_builtin(env, "fread", builtin_file_read);
  lenv_add_builtin(env, "fwrite", builtin_file_write);

  lenv_add_builtin(env, "def", builtin_def);
  lenv_add_builtin(env, "=", builtin_put);
  lenv_add_builtin(env, "fun", builtin_fun);
  lenv_add_builtin(env, "dir", builtin_dir);

  lenv_add_builtin(env, ">", builtin_greater_than);
  lenv_add_builtin(env, "<", builtin_less_than);
  lenv_add_builtin(env, ">=", builtin_greater_than_or_equals);
  lenv_add_builtin(env, "<=", builtin_less_than_or_equals);

  lenv_add_builtin(env, "==", builtin_equals);
  lenv_add_builtin(env, "!=", builtin_not_equals);

  lenv_add_builtin(env, "!", builtin_not);
  lenv_add_builtin(env, "&&", builtin_and);
  lenv_add_builtin(env, "||", builtin_or);

  lenv_add_builtin(env, "if", builtin_if);

  lenv_add_builtin(env, "+", builtin_add);
  lenv_add_builtin(env, "-", builtin_minus);
  lenv_add_builtin(env, "*", builtin_multiply);
  lenv_add_builtin(env, "/", builtin_divide);
  lenv_add_builtin(env, "%", builtin_modulo);
}

void lenv_add_builtin(lenv *env, char *name, lbuiltin function) {
  lval *key = lval_symbol(name);
  lval *value = lval_function(function, name);
  lenv_put(env, key, value);
  lval_delete(key);
  lval_delete(value);
}


lval *lenv_has_builtins(lenv *env, lval* symbols) {
 for (int i = 0; i < symbols->count; i++) {
    lval *builtin = lenv_get(env, symbols->cell[i]);

    if (builtin->type == LVAL_ERROR) {
      lval_delete(builtin);
    }

    if (builtin->type == LVAL_FUNCTION && builtin->builtin) {
      return lval_copy(builtin);
    }
  }
  return NULL;
}

