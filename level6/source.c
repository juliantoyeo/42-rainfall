#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void n(void) {
  system("/bin/cat /home/user/level7/.pass");
}

void m(void) {
  puts("Nope");
}

void main(int argc, char** argv) {
  char* str;
  void* func;

  str = malloc(64);
  func = malloc(4);

  func = &m;

  strcpy(str, argv[1]);
  func();
}