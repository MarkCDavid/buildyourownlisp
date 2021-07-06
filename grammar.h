#ifndef grammar_h
#define grammar_h

#include "mpc.h"

extern mpc_parser_t *lispy_integer;
extern mpc_parser_t *lispy_decimal;
extern mpc_parser_t *lispy_number;
extern mpc_parser_t *lispy_symbol;
extern mpc_parser_t *lispy_qexpression;
extern mpc_parser_t *lispy_sexpression;
extern mpc_parser_t *lispy_expression;
extern mpc_parser_t *lispy_core;

void lispy_load(void);
void lispy_cleanup(void);

#endif