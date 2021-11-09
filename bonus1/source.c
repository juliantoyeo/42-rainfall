#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv) {
  int nb;
  char str[40];
  
  nb = atoi(argv[1]);
  if (nb <= 9) {
    memcpy(str, argv[2], nb * 4);
    if (nb == 0x574f4c46) {
			execl("/bin/sh", "sh", 0);
		}
  } else {
    return (1);
  }
  return (0);
}