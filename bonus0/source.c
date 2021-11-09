#include <stdio.h>
#include <unistd.h>
#include <string.h>

void p(char* str, char* seperator) {
  char buffer[4096];

  puts(seperator);
  read(0, buffer, 4096);
  strchr(buffer, '\n')[0] = 0;
  strncpy(str, buffer, 20);
}

void pp(char *str) {
  char a[20];
  char b[20];

  p(a, " - ");
  p(b, " - ");
  strcpy(str, a);
  str[strlen(str)] = ' ';
  strcat(str, b);
}

int main(void) {
  char str[42];

  pp(str);
  puts(str);
  return (0);
}