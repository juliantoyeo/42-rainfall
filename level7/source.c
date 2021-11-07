#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

char c[68];

void m(void) {
  printf("%s - %d\n", c, time(0));
}

int main(int argc, char** argv) {
  int* l_var1;
  int* l_var2;

  l_var1 = malloc(8);
  l_var1[0] = 1;
  l_var1[1] = malloc(8);

  l_var2 = malloc(8);
  l_var2[0] = 2;
  l_var2[1] = malloc(8);

  strcpy(l_var1[1], argv[1]);
  strcpy(l_var2[1], argv[2]);

  fgets(c, 68, fopen("/home/user/level8/.pass", "r"));

  puts("~~");

  return (0);
}