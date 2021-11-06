#include <stdio.h>
#include <stdlib.h>

void o(void) {
  system("/bin/sh");
  _exit(1);
}

void n(void) {
  char str[520];

  fgets(str, 521, stdin);
  printf(str);
  exit(1);
}

void main(void) {
  n();
}