#include <stdio.h>

typedef struct {
  float x;
  float y;
} POINT;

void say_hello(int times) {
  for(int i = 0; i < times; i++) {
    puts("Hello, world!");
  }
}

void say_bye(int times) {
  int i = 0;
  while(i < times) {
    puts("Bye, world!");
    i++;
  }
}

void say_point(POINT point) {
  printf("(%f, %f)\n", point.x, point.y);
}

int main(int argc, char** argv) { 
  say_hello(5);
  say_bye(5);

  POINT p;
  p.x = 10.0f;
  p.y = 15.0f;

  say_point(p);
}
