#include <stdio.h>

// This is the correct usage, without using the
// typedef keyword.

struct POINT {
  float x;
  float y;
};

void say_point(struct POINT point) {
  printf("(%f, %f)\n", point.x, point.y);
}

int main(int argc, char** argv) { 

  struct POINT p;
  p.x = 10.0f;
  p.y = 15.0f;

  say_point(p);
}
