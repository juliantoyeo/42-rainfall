# Bonus 2 asm analysis

```console
greetuser()   
0x08048484 <+0>:    push   ebp  
0x08048485 <+1>:    mov    ebp,esp  
0x08048487 <+3>:    sub    esp,0x58                                 Buffer 88 bytes for local variable
0x0804848a <+6>:    mov    eax,ds:0x8049988                         copy global variable language to eax
0x0804848f <+11>:   cmp    eax,0x1                                  compare eax (language) with number 1
0x08048492 <+14>:   je     0x80484ba <greetuser+54>                 jump equal to line greetuser+54
0x08048494 <+16>:   cmp    eax,0x2                                  compare eax (language) with number 2
0x08048497 <+19>:   je     0x80484e9 <greetuser+101>                jump equal to line greetuser+101
0x08048499 <+21>:   test   eax,eax                                  test if 0
0x0804849b <+23>:   jne    0x804850a <greetuser+134>                jump not equal to line greetuser+134
0x0804849d <+25>:   mov    edx,0x8048710                            copy "Hello " to edx
0x080484a2 <+30>:   lea    eax,[ebp-0x48]                           load address [ebp-0x48] (l_var1) (should be buffer with size 72)
0x080484a5 <+33>:   mov    ecx,DWORD PTR [edx]                      copy edx ("Hello ") to ecx
0x080484a7 <+35>:   mov    DWORD PTR [eax],ecx                      copy ecx ("Hello ") to eax (l_var1)
0x080484a9 <+37>:   movzx  ecx,WORD PTR [edx+0x4] 
0x080484ad <+41>:   mov    WORD PTR [eax+0x4],cx  
0x080484b1 <+45>:   movzx  edx,BYTE PTR [edx+0x6] 
0x080484b5 <+49>:   mov    BYTE PTR [eax+0x6],dl  
                                                                    line 25 - 49 is basically doing strcpy(buffer, "Hello ")
0x080484b8 <+52>:   jmp    0x804850a <greetuser+134>                jump equal to line greetuser+134
0x080484ba <+54>:   mov    edx,0x8048717                            copy "Hyv\303\244\303\244 p\303\244iv\303\244\303\244 " to edx
0x080484bf <+59>:   lea    eax,[ebp-0x48]                           load address [ebp-0x48] (l_var1) (should be buffer with size 72)
0x080484c2 <+62>:   mov    ecx,DWORD PTR [edx]                      copy edx ("Hyv\303\244\303\244 p\303\244iv\303\244\303\244 ") to ecx
0x080484c4 <+64>:   mov    DWORD PTR [eax],ecx                      copy ecx ("Hyv\303\244\303\244 p\303\244iv\303\244\303\244 ") to eax (l_var1)
0x080484c6 <+66>:   mov    ecx,DWORD PTR [edx+0x4]  
0x080484c9 <+69>:   mov    DWORD PTR [eax+0x4],ecx  
0x080484cc <+72>:   mov    ecx,DWORD PTR [edx+0x8]  
0x080484cf <+75>:   mov    DWORD PTR [eax+0x8],ecx  
0x080484d2 <+78>:   mov    ecx,DWORD PTR [edx+0xc]  
0x080484d5 <+81>:   mov    DWORD PTR [eax+0xc],ecx  
0x080484d8 <+84>:   movzx  ecx,WORD PTR [edx+0x10]  
0x080484dc <+88>:   mov    WORD PTR [eax+0x10],cx 
0x080484e0 <+92>:   movzx  edx,BYTE PTR [edx+0x12]  
0x080484e4 <+96>:   mov    BYTE PTR [eax+0x12],dl 
                                                                    line 52 - 96 is basically doing strcpy(buffer, "Hyv\xc3\xa4\xc3\xa4 p\xc3\xa4iv\xc3\xa4\xc3\xa4 "
0x080484e7 <+99>:   jmp    0x804850a <greetuser+134>                jump equal to line greetuser+134
0x080484e9 <+101>:  mov    edx,0x804872a                            copy "Goedemiddag! " to edx
0x080484ee <+106>:  lea    eax,[ebp-0x48]                           load address [ebp-0x48] (l_var1) (should be buffer with size 72)
0x080484f1 <+109>:  mov    ecx,DWORD PTR [edx]                      copy edx ("Goedemiddag! ") to ecx
0x080484f3 <+111>:  mov    DWORD PTR [eax],ecx                      copy ecx ("Goedemiddag! ") to eax (l_var1)
0x080484f5 <+113>:  mov    ecx,DWORD PTR [edx+0x4]  
0x080484f8 <+116>:  mov    DWORD PTR [eax+0x4],ecx  
0x080484fb <+119>:  mov    ecx,DWORD PTR [edx+0x8]  
0x080484fe <+122>:  mov    DWORD PTR [eax+0x8],ecx  
0x08048501 <+125>:  movzx  edx,WORD PTR [edx+0xc] 
0x08048505 <+129>:  mov    WORD PTR [eax+0xc],dx  
                                                                    line 99 - 129 is basically doing strcpy(buffer, "Goedemiddag! ")
0x08048509 <+133>:  nop 
0x0804850a <+134>:  lea    eax,[ebp+0x8]                            load address of arg1
0x0804850d <+137>:  mov    DWORD PTR [esp+0x4],eax                  put eax (arg1) as 2nd argument for strcat
0x08048511 <+141>:  lea    eax,[ebp-0x48]                           load address of l_var1
0x08048514 <+144>:  mov    DWORD PTR [esp],eax                      put eax (l_var1) as 1st argument for strcat
0x08048517 <+147>:  call   0x8048370 <strcat@plt>                   call function strcat
0x0804851c <+152>:  lea    eax,[ebp-0x48]                           load address of l_var1
0x0804851f <+155>:  mov    DWORD PTR [esp],eax                      put eax (l_var1) as 1st argument for puts
0x08048522 <+158>:  call   0x8048390 <puts@plt>                     call function puts
0x08048527 <+163>:  leave 
0x08048528 <+164>:  ret 
    
    
main()    
0x08048529 <+0>:    push   ebp  
0x0804852a <+1>:    mov    ebp,esp  
0x0804852c <+3>:    push   edi  
0x0804852d <+4>:    push   esi  
0x0804852e <+5>:    push   ebx  
0x0804852f <+6>:    and    esp,0xfffffff0                           Bit alignment
0x08048532 <+9>:    sub    esp,0xa0                                 Buffer 160 bytes for local variable
0x08048538 <+15>:   cmp    DWORD PTR [ebp+0x8],0x3                  compare argc with 3
0x0804853c <+19>:   je     0x8048548 <main+31>                      jump equal to line main+31
0x0804853e <+21>:   mov    eax,0x1                                  return (1)
0x08048543 <+26>:   jmp    0x8048630 <main+263>                     jump to line main+263 (end the program)
0x08048548 <+31>:   lea    ebx,[esp+0x50]                           load address [esp+0x50] (l_var1) to eax
0x0804854c <+35>:   mov    eax,0x0                                  copy number 0 to eax
0x08048551 <+40>:   mov    edx,0x13                                 copy number 19 to edx
0x08048556 <+45>:   mov    edi,ebx                                  copy ebx to edi (destination index)
0x08048558 <+47>:   mov    ecx,edx                                  copy edx (19) to ecx
0x0804855a <+49>:   rep stos DWORD PTR es:[edi],eax                 repeat store string operation on eax (0) to edi
                                                                    line 31 - 49 basically mean this (memset(buffer, 0, 19 * 4)
0x0804855c <+51>:   mov    eax,DWORD PTR [ebp+0xc]                  copy [ebp+0xc] (pointer to argv) into eax
0x0804855f <+54>:   add    eax,0x4                                  add 4 bytes into eax (pointer to argv) to get argv[1]
0x08048562 <+57>:   mov    eax,DWORD PTR [eax]                      copy the content in eax (argv[1]) into eax
0x08048564 <+59>:   mov    DWORD PTR [esp+0x8],0x28                 put number 40 to 8bytes below the top of the stack, for 3rd arg for the strncpy function
0x0804856c <+67>:   mov    DWORD PTR [esp+0x4],eax                  put eax (argv[1]) to 4bytes below the top of the stack, for 2nd arg for the strncpy function
0x08048570 <+71>:   lea    eax,[esp+0x50]                           load address [esp+0x50] (l_var1) to eax (should be a buffer with 76 bytes (esp+0x9c - esp+0x50)
0x08048574 <+75>:   mov    DWORD PTR [esp],eax                      put eax (l_var1) to the top of the stack, as the 1st arg for the strncpy function
0x08048577 <+78>:   call   0x80483c0 <strncpy@plt>                  call function strncpy
0x0804857c <+83>:   mov    eax,DWORD PTR [ebp+0xc]                  copy [ebp+0xc] (pointer to argv) into eax
0x0804857f <+86>:   add    eax,0x8                                  add 8 bytes into eax (pointer to argv) to get argv[2]
0x08048582 <+89>:   mov    eax,DWORD PTR [eax]                      copy the content in eax (argv[2]) into eax
0x08048584 <+91>:   mov    DWORD PTR [esp+0x8],0x20                 put number 32 to 8bytes below the top of the stack, for 3rd arg for the strncpy function
0x0804858c <+99>:   mov    DWORD PTR [esp+0x4],eax                  put eax (argv[2]) to 4bytes below the top of the stack, for 2nd arg for the strncpy function
0x08048590 <+103>:  lea    eax,[esp+0x50]                           load address [esp+0x50] (l_var1) to eax
0x08048594 <+107>:  add    eax,0x28                                 add 40 bytes into eax (l_var1)
0x08048597 <+110>:  mov    DWORD PTR [esp],eax                      put eax (l_var1) to the top of the stack, as the 1st arg for the strncpy function
0x0804859a <+113>:  call   0x80483c0 <strncpy@plt>                  call function strncpy
0x0804859f <+118>:  mov    DWORD PTR [esp],0x8048738                put "LANG"  to the top of the stack, as the 1st arg for the getenv function
0x080485a6 <+125>:  call   0x8048380 <getenv@plt>                   call function getenv
0x080485ab <+130>:  mov    DWORD PTR [esp+0x9c],eax                 save the result of getenv into [esp+0x9c] (l_var2)
0x080485b2 <+137>:  cmp    DWORD PTR [esp+0x9c],0x0                 compare [esp+0x9c] (l_var2) with 0
0x080485ba <+145>:  je     0x8048618 <main+239>                     jump equal to line main+239
0x080485bc <+147>:  mov    DWORD PTR [esp+0x8],0x2                  put number 2 to 8bytes below the top of the stack, for 3rd arg for the memcmp function
0x080485c4 <+155>:  mov    DWORD PTR [esp+0x4],0x804873d            put "fi" to 4bytes below the top of the stack, for 2nd arg for the memcmp function
0x080485cc <+163>:  mov    eax,DWORD PTR [esp+0x9c]                 copy [esp+0x9c] (l_var2) into eax
0x080485d3 <+170>:  mov    DWORD PTR [esp],eax                      put eax (l_var2) to the top of the stack, as the 1st arg for the memcmp function
0x080485d6 <+173>:  call   0x8048360 <memcmp@plt>                   call function memcmp
0x080485db <+178>:  test   eax,eax                                  test if 0
0x080485dd <+180>:  jne    0x80485eb <main+194>                     jump not equal to line main+194
0x080485df <+182>:  mov    DWORD PTR ds:0x8049988,0x1               copy number 1 into global variable language
0x080485e9 <+192>:  jmp    0x8048618 <main+239>                     jump to line main+239
0x080485eb <+194>:  mov    DWORD PTR [esp+0x8],0x2                  put number 2 to 8bytes below the top of the stack, for 3rd arg for the memcmp function
0x080485f3 <+202>:  mov    DWORD PTR [esp+0x4],0x8048740            put "nl" to 4bytes below the top of the stack, for 2nd arg for the memcmp function
0x080485fb <+210>:  mov    eax,DWORD PTR [esp+0x9c]                 copy [esp+0x9c] (l_var2) into eax
0x08048602 <+217>:  mov    DWORD PTR [esp],eax                      put eax (l_var2) to the top of the stack, as the 1st arg for the memcmp function
0x08048605 <+220>:  call   0x8048360 <memcmp@plt>                   call function memcmp
0x0804860a <+225>:  test   eax,eax                                  test if 0
0x0804860c <+227>:  jne    0x8048618 <main+239>                     jump not equal to line main+239
0x0804860e <+229>:  mov    DWORD PTR ds:0x8049988,0x2               copy number 2 into global variable language
0x08048618 <+239>:  mov    edx,esp                                  copy esp into edx
0x0804861a <+241>:  lea    ebx,[esp+0x50]                           load address [esp+0x50] (l_var1) to ebx
0x0804861e <+245>:  mov    eax,0x13                                 copy number 19 into eax
0x08048623 <+250>:  mov    edi,edx                                  copy edx to edi (destination index)
0x08048625 <+252>:  mov    esi,ebx                                  copy ebx to esi (destination index)
0x08048627 <+254>:  mov    ecx,eax                                  copy ecx to eax
0x08048629 <+256>:  rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]  repeat copy edi to esi
                                                                    basically putting whole l_var1 to the top of the stack for function greetuser
0x0804862b <+258>:  call   0x8048484 <greetuser>                    call function greetuser
0x08048630 <+263>:  lea    esp,[ebp-0xc]  
0x08048633 <+266>:  pop    ebx  
0x08048634 <+267>:  pop    esi  
0x08048635 <+268>:  pop    edi  
0x08048636 <+269>:  pop    ebp  
0x08048637 <+270>:  ret 
```