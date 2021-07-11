#ifndef lenv_h
#define lenv_h

#include "ltypes.h"

lenv* lenv_new(void);
lenv* lenv_copy(lenv* env);
void lenv_delete(lenv* env);

lval* lenv_get(lenv* env, lval* key);
void lenv_put(lenv* env, lval* key, lval* value);
void lenv_def(lenv* env, lval* key, lval* value);

void lenv_add_builtins(lenv *env);
void lenv_add_builtin(lenv *env, char *name, lbuiltin function);
lval *lenv_has_builtins(lenv *env, lval *symbols);

#endif