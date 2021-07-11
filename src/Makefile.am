noinst_LTLIBRARIES = liblispy.la
noinst_PROGRAMS = lispy
liblispy_la_SOURCES = builtin.c builtin.h grammar.c grammar.h lenv.c lenv.h ltypes.c ltypes.h lval.c lval.h macros.h mpc.c mpc.h readline.c readline.h
liblispy_la_LIBADD = -ledit -lm
lispy_SOURCES = lispy.c
lispy_LDADD = ./liblispy.la -ledit -lm
