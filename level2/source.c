#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void p(void) {
  char str[64];
  uint ret;

  fflush(stdout);
  gets(str);

  ret = __builtin_return_address(0): // Compiler function to get the return adress

  if ((ret & 0xb0000000) == 0xb0000000) {
    printf("(%p)\n", ret);
    exit(1);
  }
  else {
    puts(str);
    strdup(str);
  }
}

void main(void) {
  p();
}