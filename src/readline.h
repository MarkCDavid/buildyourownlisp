#ifndef readline_h
#define readline_h

#ifdef _WIN32

char *readline(char *prompt);
void add_history(char *history);

#else

#include <editline/history.h>
#include <editline/readline.h>

#endif
#endif