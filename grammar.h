#include "mpc.h"

extern mpc_parser_t* lispy_number;
extern mpc_parser_t* lispy_operator;
extern mpc_parser_t* lispy_expression;
extern mpc_parser_t* lispy_core;

void lispy_load(void);
void lispy_cleanup(void);