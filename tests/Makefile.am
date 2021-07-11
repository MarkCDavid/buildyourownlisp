TESTS = check_base
check_PROGRAMS = check_base
check_base_SOURCES = tests.c tests_lval.c tests_lenv.c $(top_builddir)/src/lval.h
check_base_CFLAGS = @CHECK_CFLAGS@
check_base_LDADD = $(top_builddir)/src/liblispy.la @CHECK_LIBS@ -lm -lrt -lpthread -lsubunit
