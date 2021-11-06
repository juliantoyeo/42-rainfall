# Level 4 asm analysis

```console
p()
0x08048444 <+0>:  push   ebp  
0x08048445 <+1>:  mov    ebp,esp  
0x08048447 <+3>:  sub    esp,0x18                   Buffer 24 bytes for local variable
0x0804844a <+6>:  mov    eax,DWORD PTR [ebp+0x8]    copy [ebp+0x8] (1st arg) to eax
0x0804844d <+9>:  mov    DWORD PTR [esp],eax        put eax (1st arg) to the top of the stack, as the 1st arg for the printf function
0x08048450 <+12>: call   0x8048340 <printf@plt>     call function printf
0x08048455 <+17>: leave 
0x08048456 <+18>: ret 


n()   
0x08048457 <+0>:  push   ebp  
0x08048458 <+1>:  mov    ebp,esp  
0x0804845a <+3>:  sub    esp,0x218                  Buffer 536 bytes for local variable
0x08048460 <+9>:  mov    eax,ds:0x8049804           ds (data segment) the content is stdin, copy this to eax
0x08048465 <+14>: mov    DWORD PTR [esp+0x8],eax    put eax (stdin) to 8bytes below the top of the stack, as the 3rd arg for the fgets function
0x08048469 <+18>: mov    DWORD PTR [esp+0x4],0x200  put number 512 to 4bytes below the top of the stack, as the 2nd arg for the fgets function
0x08048471 <+26>: lea    eax,[ebp-0x208]            load address of [ebp-0x208] (pointer to local variable1) into eax (we can assume this is a buffer of 520 (ebp - 0x208 - 0x0)
0x08048477 <+32>: mov    DWORD PTR [esp],eax        put eax (pointer to local variable1) to the top of the stack, as the 1st arg for the fgets function
0x0804847a <+35>: call   0x8048350 <fgets@plt>      call function fgets
0x0804847f <+40>: lea    eax,[ebp-0x208]            load address of [ebp-0x208] (pointer to local variable1) into eax
0x08048485 <+46>: mov    DWORD PTR [esp],eax        put eax (pointer to local variable1) to the top of the stack, as the 1st arg for the p function
0x08048488 <+49>: call   0x8048444 <p>              call function p
0x0804848d <+54>: mov    eax,ds:0x8049810           ds (data segment), variable m, which content is 0 (This means that m is a global variable), copy this to eax
0x08048492 <+59>: cmp    eax,0x1025544              compare eax(variable m) with number 16930116
0x08048497 <+64>: jne    0x80484a5 <n+78>           jump not equal to line n+78
0x08048499 <+66>: mov    DWORD PTR [esp],0x8048590  put eax ("/bin/cat /home/user/level5/.pass") to the top of the stack, as the 1st arg for the system function
0x080484a0 <+73>: call   0x8048360 <system@plt>     call function system
0x080484a5 <+78>: leave 
0x080484a6 <+79>: ret 


main()    
0x080484a7 <+0>:  push   ebp  
0x080484a8 <+1>:  mov    ebp,esp  
0x080484aa <+3>:  and    esp,0xfffffff0             Bit alignment
0x080484ad <+6>:  call   0x8048457 <n>              call function n
0x080484b2 <+11>: leave 
0x080484b3 <+12>: ret 
```