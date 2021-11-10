#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
	char str[132];
	void* file = fopen("/home/user/end/.pass", "r");

	memset(str, 0, 132);
	if ((file == 0) || (argc != 2)) {
    return(-1);
  }
	else
	{
		fread(str, 1, 66, f);
		str[65] = 0;
		str[atoi(argv[1])] = 0;
		fread(&str[66], 1, 65, f);
		fclose(f);
		if (strcmp(str, argv[1]) == 0) {
      execl("/bin/sh", "sh", NULL);
    }
		else {
      puts(&str[66]);
    }
	}
	return (0);
}