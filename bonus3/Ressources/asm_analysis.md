# Bonus 3 asm analysis

```console
main()
0x080484f4 <+0>:    push   ebp  
0x080484f5 <+1>:    mov    ebp,esp  
0x080484f7 <+3>:    push   edi  
0x080484f8 <+4>:    push   ebx  
0x080484f9 <+5>:    and    esp,0xfffffff0                 Bit alignment
0x080484fc <+8>:    sub    esp,0xa0                       Buffer 160 bytes for local variable
0x08048502 <+14>:   mov    edx,0x80486f0                  copy "r" into edx
0x08048507 <+19>:   mov    eax,0x80486f2                  copy "/home/user/end/.pass" into eax
0x0804850c <+24>:   mov    DWORD PTR [esp+0x4],edx        put edx ("r") to the 2nd arg for fopen
0x08048510 <+28>:   mov    DWORD PTR [esp],eax            put eax ("/home/user/end/.pass") to the 1st arg for fopen
0x08048513 <+31>:   call   0x8048410 <fopen@plt>          call function fopen
0x08048518 <+36>:   mov    DWORD PTR [esp+0x9c],eax       save the return from fopen to [esp+0x9c] (l_var1)
0x0804851f <+43>:   lea    ebx,[esp+0x18]                 load address from [esp+0x18] (l_var2) to ebx (should be a buffer with 132 bytes (0x9c - 0x18))
0x08048523 <+47>:   mov    eax,0x0                        copy 0 to eax
0x08048528 <+52>:   mov    edx,0x21                       copy 33 to edx
0x0804852d <+57>:   mov    edi,ebx                        copy ebx to edi
0x0804852f <+59>:   mov    ecx,edx                        copy edx to ecx
0x08048531 <+61>:   rep stos DWORD PTR es:[edi],eax       repeat store string operation on eax (0) to edi
                                                          line 43 - 61 basically mean this (memset(buffer, 0, 33 * 4)
0x08048533 <+63>:   cmp    DWORD PTR [esp+0x9c],0x0       compare if [esp+0x9c] (l_var1) is 0 / null
0x0804853b <+71>:   je     0x8048543 <main+79>            jump equal to line main+79
0x0804853d <+73>:   cmp    DWORD PTR [ebp+0x8],0x2        compare if [ebp+0x8] argc iis 2
0x08048541 <+77>:   je     0x804854d <main+89>            jump equal to line main+89
0x08048543 <+79>:   mov    eax,0xffffffff                 return -1
0x08048548 <+84>:   jmp    0x8048615 <main+289>           jump to line main+289 (end program)
0x0804854d <+89>:   lea    eax,[esp+0x18]                 load address from [esp+0x18] (l_var2) to eax
0x08048551 <+93>:   mov    edx,DWORD PTR [esp+0x9c]       copy [esp+0x9c] (l_var1) into edx 
0x08048558 <+100>:  mov    DWORD PTR [esp+0xc],edx        put edx (l_var1) to the 4th arg of fread
0x0804855c <+104>:  mov    DWORD PTR [esp+0x8],0x42       put 66 to the 3rd arg of fread
0x08048564 <+112>:  mov    DWORD PTR [esp+0x4],0x1        put 1 to the 2nd arg of fread
0x0804856c <+120>:  mov    DWORD PTR [esp],eax            put eax (l_var2) to the 1st arg of fread
0x0804856f <+123>:  call   0x80483d0 <fread@plt>          call function fread
0x08048574 <+128>:  mov    BYTE PTR [esp+0x59],0x0        copy 0 into [esp+0x59] (l_var2[65])
0x08048579 <+133>:  mov    eax,DWORD PTR [ebp+0xc]        copy [ebp+0xc] (pointer to argv) into eax
0x0804857c <+136>:  add    eax,0x4                        add 4 bytes into eax (pointer to argv) to get argv[1]
0x0804857f <+139>:  mov    eax,DWORD PTR [eax]            copy the content in eax (argv[1]) into eax
0x08048581 <+141>:  mov    DWORD PTR [esp],eax            put eax (argv[1]) to the 1st arg of atoi
0x08048584 <+144>:  call   0x8048430 <atoi@plt>           call function atoi
0x08048589 <+149>:  mov    BYTE PTR [esp+eax*1+0x18],0x0  copy 0 to l_var2 + eax (return from atoi)
0x0804858e <+154>:  lea    eax,[esp+0x18]                 load address from [esp+0x18] (l_var2) to eax
0x08048592 <+158>:  lea    edx,[eax+0x42]                 load address from [eax+0x42] (l_var2 + 66) to edx
0x08048595 <+161>:  mov    eax,DWORD PTR [esp+0x9c]       copy [esp+0x9c] (l_var1) into eax
0x0804859c <+168>:  mov    DWORD PTR [esp+0xc],eax        put eax (l_var1) to the 4th arg for fread
0x080485a0 <+172>:  mov    DWORD PTR [esp+0x8],0x41       put 65 to the 3rd arg for fread
0x080485a8 <+180>:  mov    DWORD PTR [esp+0x4],0x1        put 1 to the 2nd arg of fread
0x080485b0 <+188>:  mov    DWORD PTR [esp],edx            put edx (l_var2 + 66) to the 1st arg of fread
0x080485b3 <+191>:  call   0x80483d0 <fread@plt>          call function fread
0x080485b8 <+196>:  mov    eax,DWORD PTR [esp+0x9c]       copy [esp+0x9c] (l_var1) into eax
0x080485bf <+203>:  mov    DWORD PTR [esp],eax            put eax (l_var1) to the 1st arg of fclose
0x080485c2 <+206>:  call   0x80483c0 <fclose@plt>         call function fclose
0x080485c7 <+211>:  mov    eax,DWORD PTR [ebp+0xc]        copy [ebp+0xc] (pointer to argv) into eax
0x080485ca <+214>:  add    eax,0x4                        add 4 bytes into eax (pointer to argv) to get argv[1]
0x080485cd <+217>:  mov    eax,DWORD PTR [eax]            copy the content in eax (argv[1]) into eax
0x080485cf <+219>:  mov    DWORD PTR [esp+0x4],eax        put eax (argv[1]) to the 2nd arg of strcmp
0x080485d3 <+223>:  lea    eax,[esp+0x18]                 load address from [esp+0x18] (l_var2) to eax
0x080485d7 <+227>:  mov    DWORD PTR [esp],eax            put eax (l_var2) to the 1st arg of strcmp
0x080485da <+230>:  call   0x80483b0 <strcmp@plt>         call function strcmp
0x080485df <+235>:  test   eax,eax                        test if 0
0x080485e1 <+237>:  jne    0x8048601 <main+269>           jump if not equal to line main+269
0x080485e3 <+239>:  mov    DWORD PTR [esp+0x8],0x0        put number 0 to 3rd arg of execl
0x080485eb <+247>:  mov    DWORD PTR [esp+0x4],0x8048707  put "sh" to 2nd arg of execl
0x080485f3 <+255>:  mov    DWORD PTR [esp],0x804870a      put "/bin/sh" to 1st arg of execl
0x080485fa <+262>:  call   0x8048420 <execl@plt>          call function execl
0x080485ff <+267>:  jmp    0x8048610 <main+284>           jump to line main+284 (end program)
0x08048601 <+269>:  lea    eax,[esp+0x18]                 load address from [esp+0x18] (l_var2) to eax
0x08048605 <+273>:  add    eax,0x42                       add 66bytes to l_var2
0x08048608 <+276>:  mov    DWORD PTR [esp],eax            put l_var2 as 1st arg for puts
0x0804860b <+279>:  call   0x80483e0 <puts@plt>           call function puts
0x08048610 <+284>:  mov    eax,0x0  return 0
0x08048615 <+289>:  lea    esp,[ebp-0x8]  
0x08048618 <+292>:  pop    ebx  
0x08048619 <+293>:  pop    edi  
0x0804861a <+294>:  pop    ebp  
0x0804861b <+295>:  ret 
```