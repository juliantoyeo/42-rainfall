# Level 7 asm analysis

```console
Level 6
m()
0x080484f4 <+0>:    push   ebp  
0x080484f5 <+1>:    mov    ebp,esp  
0x080484f7 <+3>:    sub    esp,0x18                       Buffer 24 bytes for local variable
0x080484fa <+6>:    mov    DWORD PTR [esp],0x0            put number 0 to the top of the stack, as the 1st arg for the time function
0x08048501 <+13>:   call   0x80483d0 <time@plt>           call function time
0x08048506 <+18>:   mov    edx,0x80486e0                  copy "%s - %d\n" into edx
0x0804850b <+23>:   mov    DWORD PTR [esp+0x8],eax        put eax (return of time) to 8bytes below the top of the stack, as the 3rd arg for the printf function
0x0804850f <+27>:   mov    DWORD PTR [esp+0x4],0x8049960  put global variable c to 4bytes below the top of the stack, as the 2nd arg for the printf function
0x08048517 <+35>:   mov    DWORD PTR [esp],edx            put edx ("%s - %d\n") to the top of the stack, as the 1st arg for the printf function
0x0804851a <+38>:   call   0x80483b0 <printf@plt>         call function printf
0x0804851f <+43>:   leave 
0x08048520 <+44>:   ret 


main()    
0x08048521 <+0>:    push   ebp  
0x08048522 <+1>:    mov    ebp,esp  
0x08048524 <+3>:    and    esp,0xfffffff0                 Bit alignment
0x08048527 <+6>:    sub    esp,0x20                       Buffer 32 bytes for local variable
0x0804852a <+9>:    mov    DWORD PTR [esp],0x8            put number 8 to the top of the stack, as the 1st arg for the malloc function
0x08048531 <+16>:   call   0x80483f0 <malloc@plt>         call function malloc
0x08048536 <+21>:   mov    DWORD PTR [esp+0x1c],eax       save eax (pointer return by malloc) into [esp+0x1c] (local variable 1)
0x0804853a <+25>:   mov    eax,DWORD PTR [esp+0x1c]       copy [esp+0x1c] (local variable 1) into eax
0x0804853e <+29>:   mov    DWORD PTR [eax],0x1            put number 1 into the address pointed by eax (local variable 1)
0x08048544 <+35>:   mov    DWORD PTR [esp],0x8            put number 8 to the top of the stack, as the 1st arg for the malloc function
0x0804854b <+42>:   call   0x80483f0 <malloc@plt>         call function malloc
0x08048550 <+47>:   mov    edx,eax                        save eax (pointer return by malloc) into edx
0x08048552 <+49>:   mov    eax,DWORD PTR [esp+0x1c]       copy [esp+0x1c] (local variable 1) into eax
0x08048556 <+53>:   mov    DWORD PTR [eax+0x4],edx        put edx (pointer return by malloc) into the address pointed by eax + 0x4 (local variable 1 + 0x4)
0x08048559 <+56>:   mov    DWORD PTR [esp],0x8            put number 8 to the top of the stack, as the 1st arg for the malloc function
0x08048560 <+63>:   call   0x80483f0 <malloc@plt>         call function malloc
0x08048565 <+68>:   mov    DWORD PTR [esp+0x18],eax       save eax (pointer return by malloc) into [esp+0x18] (local variable 2)
0x08048569 <+72>:   mov    eax,DWORD PTR [esp+0x18]       copy [esp+0x18] (local variable 2) into eax
0x0804856d <+76>:   mov    DWORD PTR [eax],0x2            put number 2 into the address pointed by eax (local variable 2)
0x08048573 <+82>:   mov    DWORD PTR [esp],0x8            put number 8 to the top of the stack, as the 1st arg for the malloc function
0x0804857a <+89>:   call   0x80483f0 <malloc@plt>         call function malloc
0x0804857f <+94>:   mov    edx,eax                        save eax (pointer return by malloc) into edx
0x08048581 <+96>:   mov    eax,DWORD PTR [esp+0x18]       copy [esp+0x18] (local variable 2) into eax
0x08048585 <+100>:  mov    DWORD PTR [eax+0x4],edx        put edx (pointer return by malloc) into the address pointed by eax + 0x4 (local variable 2 + 0x4)
0x08048588 <+103>:  mov    eax,DWORD PTR [ebp+0xc]        copy [ebp+0xc] (pointer to argv) into eax
0x0804858b <+106>:  add    eax,0x4                        add 4 bytes into eax (pointer to argv) to get argv[1]
0x0804858e <+109>:  mov    eax,DWORD PTR [eax]            copy the content in eax (argv[1]) into eax
0x08048590 <+111>:  mov    edx,eax                        copy eax (argv[1]) into edx
0x08048592 <+113>:  mov    eax,DWORD PTR [esp+0x1c]       copy [esp+0x1c] (local variable 1) into eax
0x08048596 <+117>:  mov    eax,DWORD PTR [eax+0x4]        copy [eax+0x4] (local variable 1 + 0x4) into eax
0x08048599 <+120>:  mov    DWORD PTR [esp+0x4],edx        put edx (argv[1]) to 4bytes below the top of the stack, as the 2nd arg for the strcpy function
0x0804859d <+124>:  mov    DWORD PTR [esp],eax            put eax (local variable 1 + 0x4) to the top of the stack, as the 1st arg for the strcpy function
0x080485a0 <+127>:  call   0x80483e0 <strcpy@plt>         call function strcpy
0x080485a5 <+132>:  mov    eax,DWORD PTR [ebp+0xc]        copy [ebp+0xc] (pointer to argv) into eax
0x080485a8 <+135>:  add    eax,0x8                        add 8 bytes into eax (pointer to argv) to get argv[2]
0x080485ab <+138>:  mov    eax,DWORD PTR [eax]            copy the content in eax (argv[2]) into eax
0x080485ad <+140>:  mov    edx,eax                        copy eax (argv[2]) into edx
0x080485af <+142>:  mov    eax,DWORD PTR [esp+0x18]       copy [esp+0x18] (local variable 2) into eax
0x080485b3 <+146>:  mov    eax,DWORD PTR [eax+0x4]        copy [eax+0x4] (local variable 2 + 0x4) into eax
0x080485b6 <+149>:  mov    DWORD PTR [esp+0x4],edx        put edx (argv[2]) to 4bytes below the top of the stack, as the 2nd arg for the strcpy function
0x080485ba <+153>:  mov    DWORD PTR [esp],eax            put eax (local variable 2 + 0x4) to the top of the stack, as the 1st arg for the strcpy function
0x080485bd <+156>:  call   0x80483e0 <strcpy@plt>         call function strcpy
0x080485c2 <+161>:  mov    edx,0x80486e9                  copy "r" into edx
0x080485c7 <+166>:  mov    eax,0x80486eb                  copy "/home/user/level8/.pass" into eax
0x080485cc <+171>:  mov    DWORD PTR [esp+0x4],edx        put edx ("r") to 4bytes below the top of the stack, as the 2nd arg for the fopen function
0x080485d0 <+175>:  mov    DWORD PTR [esp],eax            put eax ("/home/user/level8/.pass") to the top of the stack, as the 1st arg for the fopen function
0x080485d3 <+178>:  call   0x8048430 <fopen@plt>          call function fopen
0x080485d8 <+183>:  mov    DWORD PTR [esp+0x8],eax        put eax (return of fopen) to 8bytes below the top of the stack, as the 3rd arg for the fgets function
0x080485dc <+187>:  mov    DWORD PTR [esp+0x4],0x44       put number 68 to 4bytes below the top of the stack, as the 2nd arg for the fgets function
0x080485e4 <+195>:  mov    DWORD PTR [esp],0x8049960      put global variable c to the top of the stack, as the 1st arg for the fgets function
0x080485eb <+202>:  call   0x80483c0 <fgets@plt>          call function fgets
0x080485f0 <+207>:  mov    DWORD PTR [esp],0x8048703      put "~~" to the top of the stack, as the 1st arg for the puts function
0x080485f7 <+214>:  call   0x8048400 <puts@plt>           call function puts
0x080485fc <+219>:  mov    eax,0x0  return 0
0x08048601 <+224>:  leave 
0x08048602 <+225>:  ret 
```