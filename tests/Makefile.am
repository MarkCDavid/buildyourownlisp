TESTS = check_lispy
check_PROGRAMS = check_lispy
check_lispy_SOURCES = tests.c tests_lval.c tests_lenv.c
check_lispy_CFLAGS = @CHECK_CFLAGS@
check_lispy_LDADD = $(top_builddir)/src/liblispy.la @CHECK_LIBS@ -lm -lrt -lpthread -lsubunit
