#include <stdio.h>
#include <stdlib.h>

void run() {
  fwrite("Good... Wait what?\n", 1, 19, stdout);
  system("/bin/sh");
}

void main() {
  char str[64];

  gets(str);
}