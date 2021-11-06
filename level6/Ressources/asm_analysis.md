# Level 6 asm analysis

```console
n()
0x08048454 <+0>:  push   ebp  
0x08048455 <+1>:  mov    ebp,esp  
0x08048457 <+3>:  sub    esp,0x18                     Buffer 24 bytes for local variable
0x0804845a <+6>:  mov    DWORD PTR [esp],0x80485b0    put "/bin/cat /home/user/level7/.pass" to the top of the stack, as the 1st arg for the system function
0x08048461 <+13>: call   0x8048370 <system@plt>       call function system
0x08048466 <+18>: leave 
0x08048467 <+19>: ret 


m()
0x08048468 <+0>:  push   ebp  
0x08048469 <+1>:  mov    ebp,esp  
0x0804846b <+3>:  sub    esp,0x18                     Buffer 24 bytes for local variable
0x0804846e <+6>:  mov    DWORD PTR [esp],0x80485d1    put "Nope" to the top of the stack, as the 1st arg for the puts function
0x08048475 <+13>: call   0x8048360 <puts@plt>         call function puts
0x0804847a <+18>: leave 
0x0804847b <+19>: ret 


main()
0x0804847c <+0>:  push   ebp  
0x0804847d <+1>:  mov    ebp,esp  
0x0804847f <+3>:  and    esp,0xfffffff0               Bit alignment
0x08048482 <+6>:  sub    esp,0x20                     Buffer 32 bytes for local variable
0x08048485 <+9>:  mov    DWORD PTR [esp],0x40         put number 64 to the top of the stack, as the 1st arg for the malloc function
0x0804848c <+16>: call   0x8048350 <malloc@plt>       call function malloc
0x08048491 <+21>: mov    DWORD PTR [esp+0x1c],eax     save eax (pointer return by malloc) into [esp+0x1c] (local variable 1)
0x08048495 <+25>: mov    DWORD PTR [esp],0x4          put number 4 to the top of the stack, as the 1st arg for the malloc function
0x0804849c <+32>: call   0x8048350 <malloc@plt>       call function malloc
0x080484a1 <+37>: mov    DWORD PTR [esp+0x18],eax     save eax (pointer return by malloc) into [esp+0x18] (local variable 2)
0x080484a5 <+41>: mov    edx,0x8048468                copy 0x8048468 (address of function m) into edx
0x080484aa <+46>: mov    eax,DWORD PTR [esp+0x18]     copy [esp+0x18] (local variable 2) into eax
0x080484ae <+50>: mov    DWORD PTR [eax],edx          copy edx (address of function m) into the eax (local variable 2)
0x080484b0 <+52>: mov    eax,DWORD PTR [ebp+0xc]      copy [ebp+0xc] (pointer to argv) into eax
0x080484b3 <+55>: add    eax,0x4                      add 4 bytes into eax (pointer to argv) to get argv[1]
0x080484b6 <+58>: mov    eax,DWORD PTR [eax]          copy the content in eax (argv[1]) into eax
0x080484b8 <+60>: mov    edx,eax                      copy eax (argv[1]) into edx
0x080484ba <+62>: mov    eax,DWORD PTR [esp+0x1c]     copy [esp+0x1c] (local variable 1) into eax
0x080484be <+66>: mov    DWORD PTR [esp+0x4],edx      put edx (argv[1]) to 4bytes below the top of the stack, as the 2nd arg for the strcpy function
0x080484c2 <+70>: mov    DWORD PTR [esp],eax          put eax (local variable 1) to the top of the stack, as the 1st arg for the strcpy function
0x080484c5 <+73>: call   0x8048340 <strcpy@plt>       call function strcpy
0x080484ca <+78>: mov    eax,DWORD PTR [esp+0x18]     copy [esp+0x18] (local variable 2) into eax
0x080484ce <+82>: mov    eax,DWORD PTR [eax]          copy the content in eax (local variable 2) into eax
0x080484d0 <+84>: call   eax                          call eax (address store in local variable 2)
0x080484d2 <+86>: leave 
0x080484d3 <+87>: ret 
```