#include "grammar.h"

mpc_parser_t *lispy_integer;
mpc_parser_t *lispy_decimal;
mpc_parser_t *lispy_number;
mpc_parser_t *lispy_string;
mpc_parser_t *lispy_symbol;
mpc_parser_t *lispy_comment;
mpc_parser_t *lispy_qexpression;
mpc_parser_t *lispy_expression;
mpc_parser_t *lispy_sexpression;
mpc_parser_t *lispy_core;

void lispy_load(void) {

  lispy_integer = mpc_new("integer");
  lispy_decimal = mpc_new("decimal");
  lispy_number = mpc_new("number");
  lispy_string = mpc_new("string");
  lispy_comment = mpc_new("comment");
  lispy_symbol = mpc_new("symbol");
  lispy_qexpression = mpc_new("qexpression");
  lispy_sexpression = mpc_new("sexpression");
  lispy_expression = mpc_new("expression");
  lispy_core = mpc_new("lispy");

  FILE *grammar_file = fopen("lispy.grammar", "r");

  mpca_lang_file(MPCA_LANG_DEFAULT, grammar_file, lispy_integer, lispy_decimal,
                 lispy_number, lispy_symbol, lispy_comment, lispy_string,
                 lispy_qexpression, lispy_sexpression, lispy_expression,
                 lispy_core);

  fclose(grammar_file);
}

void lispy_cleanup(void) {
  mpc_cleanup(10, lispy_integer, lispy_decimal, lispy_number, lispy_symbol,
              lispy_comment, lispy_string, lispy_qexpression, lispy_sexpression,
              lispy_expression, lispy_core);
}