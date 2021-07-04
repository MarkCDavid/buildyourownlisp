#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readline.h"

int main(int argc, char** argv) {
  
  puts("Lispy - Type 'exit' to Exit\n");

  while(1) {
    
    char* input = readline("lispy> ");

    if(strcmp("exit", input) == 0) {
        break;
    }

    add_history(input);

    printf("%s\n", input);

    free(input);
  }

  return 0;
}
