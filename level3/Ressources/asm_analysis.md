# Level 3 asm analysis

```console
v()
0x080484a4 <+0>:    push   ebp  
0x080484a5 <+1>:    mov    ebp,esp  
0x080484a7 <+3>:    sub    esp,0x218                  Buffer 536 bytes for local variable
0x080484ad <+9>:    mov    eax,ds:0x8049860           ds (data segment) the content is stdin, copy this to eax
0x080484b2 <+14>:   mov    DWORD PTR [esp+0x8],eax    put eax (stdin) to 8bytes below the top of the stack, as the 3rd arg for the fgets function
0x080484b6 <+18>:   mov    DWORD PTR [esp+0x4],0x200  put number 512 to 4bytes below the top of the stack, as the 2nd arg for the fgets function
0x080484be <+26>:   lea    eax,[ebp-0x208]            load address of [ebp-0x208] (pointer to local variable1) into eax (we can assume this is a buffer of 520 (ebp - 0x208 - 0x0)
0x080484c4 <+32>:   mov    DWORD PTR [esp],eax        put eax (pointer to local variable1) to the top of the stack, as the 1st arg for the fgets function
0x080484c7 <+35>:   call   0x80483a0 <fgets@plt>      call function fgets
0x080484cc <+40>:   lea    eax,[ebp-0x208]            load address of [ebp-0x208] (pointer to local variable1) into eax
0x080484d2 <+46>:   mov    DWORD PTR [esp],eax        put eax (pointer to local variable1) to the top of the stack, as the 1st arg for the printf function
0x080484d5 <+49>:   call   0x8048390 <printf@plt>     call function printf
0x080484da <+54>:   mov    eax,ds:0x804988c           ds (data segment), variable m, which content is 0 (This means that m is a global variable), copy this to eax
0x080484df <+59>:   cmp    eax,0x40                   compare eax(variable m) with number 64
0x080484e2 <+62>:   jne    0x8048518 <v+116>          jump not equal to line v+116
0x080484e4 <+64>:   mov    eax,ds:0x8049880           ds (data segment) the content is stdout, copy this to eax
0x080484e9 <+69>:   mov    edx,eax                    copy eax (stdout) to edx
0x080484eb <+71>:   mov    eax,0x8048600              copy ("Wait what?!\n") to eax
0x080484f0 <+76>:   mov    DWORD PTR [esp+0xc],edx    put edx (stdout) to 12bytes below the top of the stack, as the 4th arg for the fwrite function
0x080484f4 <+80>:   mov    DWORD PTR [esp+0x8],0xc    put number 12 to 8bytes below the top of the stack, as the 3rd arg for the fwrite function
0x080484fc <+88>:   mov    DWORD PTR [esp+0x4],0x1    put number 1 to 8bytes below the top of the stack, as the 2nd arg for the fwrite function
0x08048504 <+96>:   mov    DWORD PTR [esp],eax        put eax ("Wait what?!\n") to the top of the stack, as the 1st arg for the fwrite function
0x08048507 <+99>:   call   0x80483b0 <fwrite@plt>     call function fwrite
0x0804850c <+104>:  mov    DWORD PTR [esp],0x804860d  put eax ("/bin/sh") to the top of the stack, as the 1st arg for the system function
0x08048513 <+111>:  call   0x80483c0 <system@plt>     call function system
0x08048518 <+116>:  leave 
0x08048519 <+117>:  ret 


main()    
0x0804851a <+0>:    push   ebp  
0x0804851b <+1>:    mov    ebp,esp  
0x0804851d <+3>:    and    esp,0xfffffff0             Bit alignment
0x08048520 <+6>:    call   0x80484a4 <v>              call function v
0x08048525 <+11>:   leave 
0x08048526 <+12>:   ret 
```