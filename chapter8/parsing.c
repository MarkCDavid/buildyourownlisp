#include "mpc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32

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

typedef struct {
  int type;
  double num;
  int err;
} lval;

enum { LVAL_NUM, LVAL_ERR } ;
enum { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM } ;

lval lval_num(double x) {
  lval v;
  v.type = LVAL_NUM;
  v.num = x;
  return v;
}

lval lval_err(int x) {
  lval v;
  v.type = LVAL_ERR;
  v.err = x;
  return v;
}

void lval_print(lval v) {
  switch(v.type) {
    case LVAL_NUM:
      printf("%F", v.num);
      break;
    case LVAL_ERR:
      switch(v.err) {
        case LERR_DIV_ZERO:
          printf("Error: Division by Zero!");
          break;
        case LERR_BAD_OP:
          printf("Error: Invalid Operator!");
          break;
        case LERR_BAD_NUM:
          printf("Error: Invalid Number!");
          break;
      }
      break;

  }
}

void lval_println(lval v) {
  lval_print(v);
  putchar('\n');
}

lval eval_op(lval x, char* op, lval y) {
  
  if(x.type == LVAL_ERR) { return x; }
  if(y.type == LVAL_ERR) { return y; }


  if (strcmp(op, "+") == 0) { return lval_num(x.num + y.num); }
  if (strcmp(op, "-") == 0) { return lval_num(x.num - y.num); }
  if (strcmp(op, "*") == 0) { return lval_num(x.num * y.num); }
  if (strcmp(op, "/") == 0) { 
    return y.num == 0
      ? lval_err(LERR_DIV_ZERO)
      : lval_num(x.num / y.num); 
  }
  return lval_err(LERR_BAD_OP);
}

lval eval(mpc_ast_t* t) {
  if(strstr(t->tag, "number")) {
    errno = 0;
    double x = strtod(t->contents, NULL);
    return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
  } 

  char* op = t->children[1]->contents;

  lval x = eval(t->children[2]);

  for(int i = 3; strstr(t->children[i]->tag, "expr"); i++) {
    x = eval_op(x, op, eval(t->children[i]));
  }

  return x;
}

int main(int argc, char** argv) {

  mpc_parser_t* Number = mpc_new("number");
  mpc_parser_t* Operator = mpc_new("operator");
  mpc_parser_t* Expression = mpc_new("expression");
  mpc_parser_t* Lispy = mpc_new("lispy");

  mpca_lang(MPCA_LANG_DEFAULT,
    "                                                            \
      number     : /-?([0-9]*\\.[0-9]+|[0-9]+\\.?[0-9]*)/ ;      \
      operator   : '+' | '-' | '*' | '/' ;                       \
      expression : <number> | '(' <operator> <expression>+ ')' ; \
      lispy      : /^/ <operator> <expression>+ /$/ ;            \
    ", Number, Operator, Expression, Lispy);


  puts("Lispy - Press Ctrl+c to Exit\n");

  while(1) {
    
    char* input = readline("lispy> ");

    add_history(input);

    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Lispy, &r)) {
      lval result = eval(r.output);
      lval_println(result);
      mpc_ast_delete(r.output);
    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }

    free(input);
  }

  mpc_cleanup(4, Number, Operator, Expression, Lispy);

  return 0;
}
