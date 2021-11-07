#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char	*auth = NULL;
char	*service = NULL;

int main(int argc, char** argv) {
  char str[128];

  while(1) {
    printf("%p, %p \n", edx, ecx);

    if (fgets(str, 128, stdin) == 0) {
      break;
    }
    if (strncmp(str, "auth ", 5) == 0) {
      auth = malloc(4);
      auth[0] = 0;
      if (strlen(str + 5) <= 30) {
        strcpy(auth, str + 5);
      }
    }
    if (strncmp(str, "reset", 5) == 0) {
      free(auth);
    }
    if (strncmp(str, "service", 6) == 0) {
      service = strdup(str + 7);
    }
    if (strncmp(str, "login", 5) == 0) {
      if (auth[32] != 0) {
        system("/bin/sh");
      }
      else {
        fwrite("Password:\n", 1, 10, stdout);
      }
    }
  }

  return (0);
}