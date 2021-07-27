noinst_LTLIBRARIES = liblispy.la
noinst_PROGRAMS = lispy
liblispy_la_SOURCES = builtin.c builtin.h grammar.c grammar.h lenv.c lenv.h ltypes.c ltypes.h macros.h mpc.c mpc.h readline.c readline.h lval/lval.c lval/lval.h lval/lval_integer.c lval/lval_integer.h lval/lval_decimal.c lval/lval_decimal.h lval/lval_symbol.c lval/lval_symbol.h lval/lval_string.c lval/lval_string.h lval/lval_function.c lval/lval_function.h lval/lval_error.c lval/lval_error.h lval/lval_sexpression.c lval/lval_sexpression.h lval/lval_qexpression.c lval/lval_qexpression.h lval/lval_lambda.c lval/lval_lambda.h lval/lval_exit.c lval/lval_exit.h lval/lval_file.c lval/lval_file.h lval/lval_ok.c lval/lval_ok.h
liblispy_la_LIBADD = -ledit -lm
lispy_SOURCES = lispy.c
lispy_LDADD = ./liblispy.la -ledit -lm
