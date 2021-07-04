#include "grammar.h"

mpc_parser_t* lispy_number;
mpc_parser_t* lispy_operator;
mpc_parser_t* lispy_expression;
mpc_parser_t* lispy_core;

void lispy_load(void) {

  lispy_number = mpc_new("number");
  lispy_operator = mpc_new("operator");
  lispy_expression = mpc_new("expression");
  lispy_core = mpc_new("lispy");

  FILE* grammar_file = fopen("lispy.grammar", "r");
  
  mpca_lang_file(
    MPCA_LANG_DEFAULT, 
    grammar_file, 
    lispy_number, lispy_operator, lispy_expression, lispy_core
  );

  fclose(grammar_file);
}

void lispy_cleanup(void) {
  mpc_cleanup(4, lispy_number, lispy_operator, lispy_expression, lispy_core);
}