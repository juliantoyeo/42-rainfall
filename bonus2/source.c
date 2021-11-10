#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int language = 0;

void greetuser(char *arg) {
	char str[72];

	if (language == 1) {
		strcpy(str, "Hyv\xc3\xa4\xc3\xa4 p\xc3\xa4iv\xc3\xa4\xc3\xa4 ");
	}
	else if (language == 2) {
		strcpy(str, "Goedemiddag! ");
	}
	else if (language == 0) {
		strcpy(str, "Hello ");
	}
	strcat(str, arg);
	puts(str);
}

int main(int ac, char **av) {
	char	str[76];
	char	*env = NULL;

	if (ac != 3) {
    return (1);
  }
	memset(str, 0, 19 * 4);
	strncpy(str, av[1], 40);
	strncpy(str + 40, av[2], 32);
	env = getenv("LANG");
	if (env != 0) {
		if (memcmp(env, "fi", 2) == 0) {
			lang = 1;
		}
    else if (memcmp(env, "nl", 2) == 0) {
			lang = 2;
		}
	}
	greetuser(str);
	return (0);
}