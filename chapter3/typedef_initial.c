#include <stdio.h>

// Given that typedef simply provides an alias
// for a specified name, all code areas that
// use POINT could be replaced with the struct
// definition.

typedef struct {
  float x;
  float y;
} POINT;

void say_point(POINT point) {
  printf("(%f, %f)\n", point.x, point.y);
}

int main(int argc, char** argv) { 

  POINT p;
  p.x = 10.0f;
  p.y = 15.0f;

  say_point(p);
}
