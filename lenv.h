#ifndef lenv_h
#define lenv_h

#include "ltypes.h"

lenv* lenv_new(void);
void lenv_delete(lenv* env);

lval* lenv_get(lenv* env, lval* key);
void lenv_put(lenv* env, lval* key, lval* value);

void lenv_add_builtins(lenv *env);
void lenv_add_builtin(lenv *env, char *name, lbuiltin function);

#endif