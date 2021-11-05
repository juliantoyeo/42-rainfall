# Level 0

<h3>source.c</h3>

```console
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
```

<h3>Step</h3>

Base on the decompiled c code, we can see that as long as the input that we give to the program is equal to 423, it will spawn a shell

<h3>Exploit</h3>

```console
level0@RainFall:~$ ./level0 423
$ whoami
level1
$ cat /home/user/level1/.pass
1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a
```