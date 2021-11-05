#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char** argv){

  char* execv_arg[2];
  uid_t l_var3;
  gid_t l_var4;
  
  if (atoi(argv[1]) != 423) {
    fwrite("No !\n", 1, 5, stderr);
  }
  else {
    execv_arg[0] = strdup("/bin/sh");
    execv_arg[1] = 0;
    l_var3 = getegid();
    l_var4 = geteuid();

    setresgid(l_var3, l_var3, l_var3);
    setresuid(l_var4, l_var4, l_var4);

    execv("/bin/sh", execv_arg);
  }

  return (0);
}