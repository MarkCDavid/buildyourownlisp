#include <stdio.h>

#define INPUT_SIZE 2048

static char input[INPUT_SIZE];

int main(int argc, char** argv) {
  
  puts("Lispy - Press Ctrl+c to Exit\n");

  while(1) {
    
    fputs("lispy> ", stdout);

    fgets(input, INPUT_SIZE, stdin);

    printf("You said: %s", input);
  }

  return 0;
}
