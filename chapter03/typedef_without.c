#include <stdio.h>

// Alas, I was incorrect. The usage should
// be different.

void say_point(struct { float x; float y; } point) {
  printf("(%f, %f)\n", point.x, point.y);
}

int main(int argc, char** argv) { 

  struct { float x; float y; } p;
  p.x = 10.0f;
  p.y = 15.0f;

  say_point(p);
}
