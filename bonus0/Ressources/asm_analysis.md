# Bonus 0 asm analysis

```console
Bonus 0   
p()   
0x080484b4 <+0>:    push   ebp  
0x080484b5 <+1>:    mov    ebp,esp  
0x080484b7 <+3>:    sub    esp,0x1018                         Buffer 4120 bytes for local variable
0x080484bd <+9>:    mov    eax,DWORD PTR [ebp+0xc]            copy [ebp+0xc] (arg2) into eax
0x080484c0 <+12>:   mov    DWORD PTR [esp],eax                put eax (arg2) to the top of the stack, as the 1st arg for the puts function
0x080484c3 <+15>:   call   0x80483b0 <puts@plt>               call function puts
0x080484c8 <+20>:   mov    DWORD PTR [esp+0x8],0x1000         put number 4096 to 8bytes below the top of the stack, as the 3rd arg for the read function
0x080484d0 <+28>:   lea    eax,[ebp-0x1008]                   load address of [ebp-0x1008] (pointer to local variable1) into eax (we can assume this is a buffer of 4096
0x080484d6 <+34>:   mov    DWORD PTR [esp+0x4],eax            put eax (pointer to local variable1) to 4bytes below the top of the stack, as the 2nd arg for the read function
0x080484da <+38>:   mov    DWORD PTR [esp],0x0                put number 0 to the top of the stack, as the 1st arg for the read function
0x080484e1 <+45>:   call   0x8048380 <read@plt>               call function read
0x080484e6 <+50>:   mov    DWORD PTR [esp+0x4],0xa            put "\n" to 4bytes below the top of the stack, as the 2nd arg for the strchr function
0x080484ee <+58>:   lea    eax,[ebp-0x1008]                   load address of [ebp-0x1008] (pointer to local variable1) into eax
0x080484f4 <+64>:   mov    DWORD PTR [esp],eax                put eax (pointer to local variable1) to the top of the stack, as the 1st arg for the strchr function
0x080484f7 <+67>:   call   0x80483d0 <strchr@plt>             call function strchr
0x080484fc <+72>:   mov    BYTE PTR [eax],0x0                 copy number 0 into eax[0]
0x080484ff <+75>:   lea    eax,[ebp-0x1008]                   load address of [ebp-0x1008] (pointer to local variable1) into eax
0x08048505 <+81>:   mov    DWORD PTR [esp+0x8],0x14           put number 20 to 8bytes below the top of the stack, as the 3rd arg for the strncpy function
0x0804850d <+89>:   mov    DWORD PTR [esp+0x4],eax            put eax (pointer to local variable1) to 4bytes below the top of the stack, as the 2nd arg for the strncpy function
0x08048511 <+93>:   mov    eax,DWORD PTR [ebp+0x8]            copy [ebp+0x8] (arg1) into eax
0x08048514 <+96>:   mov    DWORD PTR [esp],eax                put eax (arg1) to the top of the stack, as the 1st arg for the strncpy function
0x08048517 <+99>:   call   0x80483f0 <strncpy@plt>            call function strncpy
0x0804851c <+104>:  leave 
0x0804851d <+105>:  ret 


pp()    
0x0804851e <+0>:    push   ebp  
0x0804851f <+1>:    mov    ebp,esp  
0x08048521 <+3>:    push   edi  
0x08048522 <+4>:    push   ebx  
0x08048523 <+5>:    sub    esp,0x50                         Buffer 80 bytes for local variable
0x08048526 <+8>:    mov    DWORD PTR [esp+0x4],0x80486a0    put " - " to 4bytes below the top of the stack, as the 2nd arg for the p function
0x0804852e <+16>:   lea    eax,[ebp-0x30]                   load address of [ebp-0x30] (pointer to local variable1) into eax (we can assume this is a buffer of 32 (0x50 - 0x30)
0x08048531 <+19>:   mov    DWORD PTR [esp],eax              put eax (pointer local variable 1) to the top of the stack, as the 1st arg for the p function
0x08048534 <+22>:   call   0x80484b4 <p>                    call function p
0x08048539 <+27>:   mov    DWORD PTR [esp+0x4],0x80486a0    put  " - " to 4bytes below the top of the stack, as the 2nd arg for the p function
0x08048541 <+35>:   lea    eax,[ebp-0x1c]                   load address of [ebp-0x1c] (pointer to local variable2) into eax (we can assume this is a buffer of 20 (0x30 - 0x1c)
0x08048544 <+38>:   mov    DWORD PTR [esp],eax              put eax (pointer local variable 2) to the top of the stack, as the 1st arg for the p function
0x08048547 <+41>:   call   0x80484b4 <p>                    call function p
0x0804854c <+46>:   lea    eax,[ebp-0x30]                   load address of [ebp-0x30] (pointer to local variable1) into eax
0x0804854f <+49>:   mov    DWORD PTR [esp+0x4],eax          put eax (pointer local variable 1) to 4bytes below the top of the stack, as the 2nd arg for the strcpy function
0x08048553 <+53>:   mov    eax,DWORD PTR [ebp+0x8]          copy [ebp+0x8] (arg1) into eax
0x08048556 <+56>:   mov    DWORD PTR [esp],eax              put eax (arg1) to the top of the stack, as the 1st arg for the strcpy function
0x08048559 <+59>:   call   0x80483a0 <strcpy@plt>           call function strcpy
0x0804855e <+64>:   mov    ebx,0x80486a4                    copy " " into ebx
0x08048563 <+69>:   mov    eax,DWORD PTR [ebp+0x8]          copy [ebp+0x8] (arg1) into eax
0x08048566 <+72>:   mov    DWORD PTR [ebp-0x3c],0xffffffff  put number -1 into [ebp-0x3c]
0x0804856d <+79>:   mov    edx,eax                          copy eax (arg1) into edx
0x0804856f <+81>:   mov    eax,0x0                          copy number 0 into eax
0x08048574 <+86>:   mov    ecx,DWORD PTR [ebp-0x3c]         copy [ebp-0x3c] (-1) into ecx
0x08048577 <+89>:   mov    edi,edx                          copy edx (arg1) to edi (destionation index)
0x08048579 <+91>:   repnz scas al,BYTE PTR es:[edi]         repeat not zero Scan String Operands (scas) on edi, repeat until zero flag is not set and cx is not zero,
0x0804857b <+93>:   mov    eax,ecx                          copy ecx (counter) into eax
0x0804857d <+95>:   not    eax                              reverse the bit inside eax (counter)
0x0804857f <+97>:   sub    eax,0x1                          since ecx counts down from 0xffffffff (also known as -1)
                                                            line 69-97 is basically doing strlen(arg1)
0x08048582 <+100>:  add    eax,DWORD PTR [ebp+0x8]          add [ebp+0x8] (arg1) + eax, result will be arg1 + eax(result from strlen) or arg1[strlen(arg1)]
0x08048585 <+103>:  movzx  edx,WORD PTR [ebx]               move zero extend [ebx] (" ") into edx (arg1)
0x08048588 <+106>:  mov    WORD PTR [eax],dx                not really sure :(
0x0804858b <+109>:  lea    eax,[ebp-0x1c]                   load address of [ebp-0x1c] (pointer to local variable2) into eax
0x0804858e <+112>:  mov    DWORD PTR [esp+0x4],eax          put eax (pointer to local variable2) to 4bytes below the top of the stack, as the 2nd arg for the strcat function
0x08048592 <+116>:  mov    eax,DWORD PTR [ebp+0x8]          copy [ebp+0x8] (arg1) into eax
0x08048595 <+119>:  mov    DWORD PTR [esp],eax              put eax (arg1) to the top of the stack, as the 1st arg for the strcat function
0x08048598 <+122>:  call   0x8048390 <strcat@plt>           call function strcat
0x0804859d <+127>:  add    esp,0x50 
0x080485a0 <+130>:  pop    ebx  
0x080485a1 <+131>:  pop    edi  
0x080485a2 <+132>:  pop    ebp  
0x080485a3 <+133>:  ret 


main()    
0x080485a4 <+0>:    push   ebp  
0x080485a5 <+1>:    mov    ebp,esp  
0x080485a7 <+3>:    and    esp,0xfffffff0 Bit alignment
0x080485aa <+6>:    sub    esp,0x40                         Buffer 64 bytes for local variable
0x080485ad <+9>:    lea    eax,[esp+0x16]                   load address of [esp+0x16] (pointer to local variable1) into eax (we can assume this is a buffer of 42 (0x40 - 0x16)
0x080485b1 <+13>:   mov    DWORD PTR [esp],eax              put eax (pointer local variable 1) to the top of the stack, as the 1st arg for the pp function
0x080485b4 <+16>:   call   0x804851e <pp>                   call function pp
0x080485b9 <+21>:   lea    eax,[esp+0x16]                   load address of [esp+0x16] (pointer to local variable1) into eax
0x080485bd <+25>:   mov    DWORD PTR [esp],eax              put eax (pointer local variable 1) to the top of the stack, as the 1st arg for the puts function
0x080485c0 <+28>:   call   0x80483b0 <puts@plt>             call function puts
0x080485c5 <+33>:   mov    eax,0x0  return 0
0x080485ca <+38>:   leave 
0x080485cb <+39>:   ret 
```