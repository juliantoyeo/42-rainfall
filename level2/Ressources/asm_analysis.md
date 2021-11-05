# Level 2 asm analysis

<h3>p()</h3>

```console
0x080484d4 <+0>:    push   ebp  
0x080484d5 <+1>:    mov    ebp,esp  
0x080484d7 <+3>:    sub    esp,0x68                 Buffer 104 bytes for local variable
0x080484da <+6>:    mov    eax,ds:0x8049860         ds (data segment) the content is stdout, copy this result to eax
0x080484df <+11>:   mov    DWORD PTR [esp],eax      put eax (stdout) to the top of the stack, as the 1st arg for the fflush function
0x080484e2 <+14>:   call   0x80483b0 <fflush@plt>   call function fflush
0x080484e7 <+19>:   lea    eax,[ebp-0x4c]           load address of [ebp-0x4c] (pointer to local variable1) into eax (since gets expect a char*, we can assume this is have a buffer of 64 (ebp - 0x4c - 0xc)
0x080484ea <+22>:   mov    DWORD PTR [esp],eax      put eax (pointer to local variable1) to the top of the stack, as the 1st arg for the gets function
0x080484ed <+25>:   call   0x80483c0 <gets@plt>     call function gets
0x080484f2 <+30>:   mov    eax,DWORD PTR [ebp+0x4]  copy [ebp+0x4] (RET) to eax
0x080484f5 <+33>:   mov    DWORD PTR [ebp-0xc],eax  copy eax (RET) to [ebp-0xc] (local variable2)
0x080484f8 <+36>:   mov    eax,DWORD PTR [ebp-0xc]  copy [ebp-0xc] (local variable2) to eax
0x080484fb <+39>:   and    eax,0xb0000000           mask eax (local variable2) with 0xb0000000
0x08048500 <+44>:   cmp    eax,0xb0000000           compare if eax (local variable2) is the same as 0xb0000000
0x08048505 <+49>:   jne    0x8048527 <p+83>         jump not equal to line p+83
0x08048507 <+51>:   mov    eax,0x8048620            copy ("(%p)\n") to eax
0x0804850c <+56>:   mov    edx,DWORD PTR [ebp-0xc]  copy [ebp-0xc] (local variable2) to edx
0x0804850f <+59>:   mov    DWORD PTR [esp+0x4],edx  put edx (local variable2) to 4bytes below the top of the stack, as the 2nd arg for the printf function
0x08048513 <+63>:   mov    DWORD PTR [esp],eax      put eax ("(%p)\n") to the top of the stack, as the 1st arg for the printf function
0x08048516 <+66>:   call   0x80483a0 <printf@plt>   call function printf
0x0804851b <+71>:   mov    DWORD PTR [esp],0x1      put number 1 to the top of the stack, as the 1st arg for the exit function
0x08048522 <+78>:   call   0x80483d0 <_exit@plt>    call function exit
0x08048527 <+83>:   lea    eax,[ebp-0x4c]           load address of [ebp-0x4c] (pointer to local variable1) into eax
0x0804852a <+86>:   mov    DWORD PTR [esp],eax      put eax (pointer to local variable1) to the top of the stack, as the 1st arg for the puts function
0x0804852d <+89>:   call   0x80483f0 <puts@plt>     call function puts
0x08048532 <+94>:   lea    eax,[ebp-0x4c]           load address of [ebp-0x4c] (pointer to local variable1) into eax
0x08048535 <+97>:   mov    DWORD PTR [esp],eax      put eax (pointer to local variable1) to the top of the stack, as the 1st arg for the strdup function
0x08048538 <+100>:  call   0x80483e0 <strdup@plt>   call function strdup
0x0804853d <+105>:  leave 
0x0804853e <+106>:  ret 
```

<h3>main()</h3>

```console
0x0804853f <+0>:    push   ebp  
0x08048540 <+1>:    mov    ebp,esp  
0x08048542 <+3>:    and    esp,0xfffffff0           Bit alignment
0x08048545 <+6>:    call   0x80484d4 <p>            call function p
0x0804854a <+11>:   leave 
0x0804854b <+12>:   ret
```