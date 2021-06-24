#include "mpc.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <string.h>

static char buffer[2048];
char* readline(char* prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char* input = malloc(strlen(buffer)+1);
  strcpy(input, buffer);
  input[strlen(input) - 1] = '\0';
  return input;
}

void add_history(char* history) {}

#else
#include <editline/readline.h>
#include <editline/history.h>
#endif

int main(int argc, char** argv) {

  mpc_parser_t* Number = mpc_new("num");
  mpc_parser_t* NamedOperator = mpc_new("named_op");
  mpc_parser_t* Operator = mpc_new("op");
  mpc_parser_t* ParenthesizedExpression = mpc_new("pexpr");
  mpc_parser_t* Expression = mpc_new("expr");
  mpc_parser_t* Lispy = mpc_new("lispy");

  mpca_lang(MPCA_LANG_DEFAULT,
    "                                                                                       \
      num      : /-?([0-9]*\\.[0-9]+|[0-9]+\\.?[0-9]*)/                                   ; \
      named_op : \"add\" | \"sub\" | \"mul\" | \"div\" | \"mod\"                          ; \
      op       : '+' | '-' | '*' | '/' | '%' | <named_op>                                 ; \
      pexpr    : <num> <op> '(' <expr> ')' | '(' <expr> ')' <op> <expr> | '(' <expr> ')'  ; \
      expr     : <num> <op> <expr> | <pexpr> | <num>                                      ; \
      lispy    : /^/ <expr> /$/                                                           ; \
    ", Number, NamedOperator, Operator, ParenthesizedExpression, Expression, Lispy);


  puts("Lispy - Press Ctrl+c to Exit\n");

  while(1) {
    
    char* input = readline("lispy> ");

    add_history(input);

    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Lispy, &r)) {
      mpc_ast_print(r.output);
      mpc_ast_delete(r.output);
    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }

    free(input);
  }

  mpc_cleanup(6, Number, NamedOperator, Operator, ParenthesizedExpression, Expression, Lispy);

  return 0;
}
