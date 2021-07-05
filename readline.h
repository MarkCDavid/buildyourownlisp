#ifndef readline_h
#define readline_h

#ifdef _WIN32

char* readline(char* prompt);
void add_history(char* history);

#else

#include <editline/readline.h>
#include <editline/history.h>

#endif
#endif