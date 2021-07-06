#include "lenv.h"
#include "lval.h"
#include "builtin.h"
#include <stdlib.h>

lenv *lenv_new(void) {
  lenv *env = malloc(sizeof(lenv));
  env->count = 0;
  env->symbols = NULL;
  env->values = NULL;
  return env;
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
  return lval_error("Unbound symbol '%s'!", key->symbol);
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

void lenv_add_builtins(lenv *env) {
  lenv_add_builtin(env, "list", builtin_list);
  lenv_add_builtin(env, "head", builtin_head);
  lenv_add_builtin(env, "tail", builtin_tail);
  lenv_add_builtin(env, "eval", builtin_eval);
  lenv_add_builtin(env, "join", builtin_join);
  lenv_add_builtin(env, "cons", builtin_cons);
  lenv_add_builtin(env, "len", builtin_len);
  lenv_add_builtin(env, "init", builtin_init);
  lenv_add_builtin(env, "def", builtin_def);

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
