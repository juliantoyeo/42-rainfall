# Level 8 asm analysis

```console
main()
0x08048564 <+0>:    push   ebp  
0x08048565 <+1>:    mov    ebp,esp  
0x08048567 <+3>:    push   edi  
0x08048568 <+4>:    push   esi  
0x08048569 <+5>:    and    esp,0xfffffff0               Bit alignment
0x0804856c <+8>:    sub    esp,0xa0                     Buffer 160 bytes for local variable
0x08048572 <+14>:   jmp    0x8048575 <main+17>          Jump to line main+17
0x08048574 <+16>:   nop                                 No operation
0x08048575 <+17>:   mov    ecx,DWORD PTR ds:0x8049ab0   copy address of global variable service into ecx
0x0804857b <+23>:   mov    edx,DWORD PTR ds:0x8049aac   copy address of global variable auth into edx
0x08048581 <+29>:   mov    eax,0x8048810                copy "%p, %p \n" into eax
0x08048586 <+34>:   mov    DWORD PTR [esp+0x8],ecx      put ecx (address of global variable service) to 8bytes below the top of the stack, as the 3rd arg for the printf function
0x0804858a <+38>:   mov    DWORD PTR [esp+0x4],edx      put edx (address of global variable auth) to 4bytes below the top of the stack, as the 2nd arg for the printf function
0x0804858e <+42>:   mov    DWORD PTR [esp],eax          put eax ("%p, %p \n") to the top of the stack, as the 1st arg for the printf function
0x08048591 <+45>:   call   0x8048410 <printf@plt>       call function printf
0x08048596 <+50>:   mov    eax,ds:0x8049a80             put stdin into eax
0x0804859b <+55>:   mov    DWORD PTR [esp+0x8],eax      put eax (stdin) to 8bytes below the top of the stack, as the 3rd arg for the fgets function
0x0804859f <+59>:   mov    DWORD PTR [esp+0x4],0x80     put number 128 to 4bytes below the top of the stack, as the 2nd arg for the fgets function
0x080485a7 <+67>:   lea    eax,[esp+0x20]               load address of [esp+0x20] (pointer to local variable1) into eax
0x080485ab <+71>:   mov    DWORD PTR [esp],eax          put eax (pointer to local variable1) to the top of the stack, as the 1st arg for the fgets function
0x080485ae <+74>:   call   0x8048440 <fgets@plt>        call function fgets
0x080485b3 <+79>:   test   eax,eax                      check if the return of fgets is equal to 0
0x080485b5 <+81>:   je     0x804872c <main+456>         jump if equal to line main+456 (this loops like a loop break base on analysis on line main+376
0x080485bb <+87>:   lea    eax,[esp+0x20]               load address of [esp+0x20] (pointer to local variable1) into eax
0x080485bf <+91>:   mov    edx,eax                      copy the eax (pointer to local variable1) to edx
0x080485c1 <+93>:   mov    eax,0x8048819                copy "auth " into eax
0x080485c6 <+98>:   mov    ecx,0x5                      copy number 5 into ecx
0x080485cb <+103>:  mov    esi,edx                      copy edx (pointer to local variable1) into esi (source index)
0x080485cd <+105>:  mov    edi,eax                      copy eax ("auth ") into edi (destination index)
0x080485cf <+107>:  repz   cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
                                                        repeat Compare String Operands (cmps) on esi and edi
0x080485d1 <+109>:  seta   dl                           set dl (lower part of dx) to 1 if the above flag is set, or dl to 0 if not
0x080485d4 <+112>:  setb   al                           set al (lower part of ax) to 1 if the below flag is set, or al to 0 if not
0x080485d7 <+115>:  mov    ecx,edx                      copy edx (pointer to local variable1) into ecx
0x080485d9 <+117>:  sub    cl,al                        subtract al (lower part of ax) with cl (lower part of cx)
0x080485db <+119>:  mov    eax,ecx                      copy ecx (after the lower part gets substracted by al) into eax
0x080485dd <+121>:  movsx  eax,al                       movsx is to copy a smaller memory to bigger memory by sign extension (al is lower part of ax)
0x080485e0 <+124>:  test   eax,eax                      all those operation above is just to check if the result of the cmp is equal to 0
0x080485e2 <+126>:  jne    0x8048642 <main+222>         jump not equal to line main+222
0x080485e4 <+128>:  mov    DWORD PTR [esp],0x4          put number 4 to the top of the stack, as the 1st arg for the malloc function
0x080485eb <+135>:  call   0x8048470 <malloc@plt>       call function malloc
0x080485f0 <+140>:  mov    ds:0x8049aac,eax             save the eax (pointer return by malloc) into global variable auth
0x080485f5 <+145>:  mov    eax,ds:0x8049aac             copy global variable auth into eax
0x080485fa <+150>:  mov    DWORD PTR [eax],0x0          put number 0 into eax (global variable auth)
0x08048600 <+156>:  lea    eax,[esp+0x20]               load address of [esp+0x20] (pointer to local variable1) into eax
0x08048604 <+160>:  add    eax,0x5                      add 5 bytes to eax (pointer to local variable1)
0x08048607 <+163>:  mov    DWORD PTR [esp+0x1c],0xffffffff
                                                        put number -1 into [esp+0x1c]
0x0804860f <+171>:  mov    edx,eax                      copy eax (pointer to local variable1 + 5) into edx
0x08048611 <+173>:  mov    eax,0x0                      copy number 0 into eax
0x08048616 <+178>:  mov    ecx,DWORD PTR [esp+0x1c]     copy [esp+0x1c] (-1) into ecx
0x0804861a <+182>:  mov    edi,edx                      copy edx (pointer to local variable1 + 5) to edi (destionation index)
0x0804861c <+184>:  repnz   scas al,BYTE PTR es:[edi]   repeat not zero Scan String Operands (scas) on edi, repeat until zero flag is not set and cx is not zero,
                                                        each iteration decrements ecx, since ecx start from -1 (max int) it will make sure to loop the max time possible
                                                        scas compare each bytes in edi vs al, since eax was set to 0 in line +173, al the lower part of ax must be 0 as well
                                                        as long as the edi[i] is not 0, the loop will keep repeating, (al vs edi[i]) -> (0 vs edi[i])
                                                        zero flag will be set if the two values equal and the operation will stop, and ecx will contain the total count of the loop, thus the total length of edi
0x0804861e <+186>:  mov    eax,ecx                      copy ecx (counter) into eax
0x08048620 <+188>:  not    eax                          reverse the bit inside eax (counter)
0x08048622 <+190>:  sub    eax,0x1                      since ecx counts down from 0xffffffff (also known as -1), all the operation from line 186-190 is to get the real counter result from the repnz scas
0x08048625 <+193>:  cmp    eax,0x1e                     compare eax with number 30
0x08048628 <+196>:  ja     0x8048642 <main+222>         jump if above to line main+222
0x0804862a <+198>:  lea    eax,[esp+0x20]               load address of [esp+0x20] (pointer to local variable1) into eax
0x0804862e <+202>:  lea    edx,[eax+0x5]                add 5 bytes to eax (pointer to local variable1) and save it into edx
0x08048631 <+205>:  mov    eax,ds:0x8049aac             copy address of global variable auth into eax
0x08048636 <+210>:  mov    DWORD PTR [esp+0x4],edx      put edx (pointer to local variable1 + 5)to 4bytes below the top of the stack, as the 2nd arg for the strcpy function
0x0804863a <+214>:  mov    DWORD PTR [esp],eax          put eax (global variable auth) to the top of the stack, as the 1st arg for the strcpy function
0x0804863d <+217>:  call   0x8048460 <strcpy@plt>       call function strcpy
0x08048642 <+222>:  lea    eax,[esp+0x20]               load address of [esp+0x20] (pointer to local variable1) into eax
0x08048646 <+226>:  mov    edx,eax                      copy the eax (pointer to local variable1) to edx
0x08048648 <+228>:  mov    eax,0x804881f                copy "reset" into eax
0x0804864d <+233>:  mov    ecx,0x5                      copy number 5 into ecx
0x08048652 <+238>:  mov    esi,edx                      copy edx (pointer to local variable1) into esi (source index)
0x08048654 <+240>:  mov    edi,eax                      copy eax ("reset") into edi (destination index)
0x08048656 <+242>:  repz   cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
                                                        repeat Compare String Operands (cmps) on esi and edi
0x08048658 <+244>:  seta   dl                           set dl (lower part of dx) to 1 if the above flag is set, or dl to 0 if not
0x0804865b <+247>:  setb   al                           set al (lower part of ax) to 1 if the below flag is set, or al to 0 if not
0x0804865e <+250>:  mov    ecx,edx                      copy edx (pointer to local variable1) into ecx
0x08048660 <+252>:  sub    cl,al                        subtract al (lower part of ax) with cl (lower part of cx)
0x08048662 <+254>:  mov    eax,ecx                      copy ecx (after the lower part gets substracted by al) into eax
0x08048664 <+256>:  movsx  eax,al movsx is to           copy a smaller memory to bigger memory by sign extension (al is lower part of ax)
0x08048667 <+259>:  test   eax,eax                      all those operation above is just to check if the result of the cmp is equal to 0
0x08048669 <+261>:  jne    0x8048678 <main+276>         jump not equal to line main+276
0x0804866b <+263>:  mov    eax,ds:0x8049aac             copy address of global variable auth into eax
0x08048670 <+268>:  mov    DWORD PTR [esp],eax          put eax (global variable auth) to the top of the stack, as the 1st arg for the free function
0x08048673 <+271>:  call   0x8048420 <free@plt>         call function free
0x08048678 <+276>:  lea    eax,[esp+0x20]               load address of [esp+0x20] (pointer to local variable1) into eax
0x0804867c <+280>:  mov    edx,eax                      copy the eax (pointer to local variable1) to edx
0x0804867e <+282>:  mov    eax,0x8048825                copy "service" into eax
0x08048683 <+287>:  mov    ecx,0x6                      copy number 6 into ecx
0x08048688 <+292>:  mov    esi,edx                      copy edx (pointer to local variable1) into esi (source index)
0x0804868a <+294>:  mov    edi,eax                      copy eax ("service") into edi (destination index)
0x0804868c <+296>:  repz   cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
                                                        repeat Compare String Operands (cmps) on esi and edi
0x0804868e <+298>:  seta   dl                           set dl (lower part of dx) to 1 if the above flag is set, or dl to 0 if not
0x08048691 <+301>:  setb   al                           set al (lower part of ax) to 1 if the below flag is set, or al to 0 if not
0x08048694 <+304>:  mov    ecx,edx                      copy edx (pointer to local variable1) into ecx
0x08048696 <+306>:  sub    cl,al                        subtract al (lower part of ax) with cl (lower part of cx)
0x08048698 <+308>:  mov    eax,ecx                      copy ecx (after the lower part gets substracted by al) into eax
0x0804869a <+310>:  movsx  eax,al movsx is to           copy a smaller memory to bigger memory by sign extension (al is lower part of ax)
0x0804869d <+313>:  test   eax,eax                      all those operation above is just to check if the result of the cmp is equal to 0
0x0804869f <+315>:  jne    0x80486b5 <main+337>         jump not equal to line main+337
0x080486a1 <+317>:  lea    eax,[esp+0x20]               load address of [esp+0x20] (pointer to local variable1) into eax
0x080486a5 <+321>:  add    eax,0x7                      add 7 bytes to eax (pointer to local variable1)
0x080486a8 <+324>:  mov    DWORD PTR [esp],eax          put eax (pointer to local variable1 + 7) to the top of the stack, as the 1st arg for the strdup function
0x080486ab <+327>:  call   0x8048430 <strdup@plt>       call function strdup
0x080486b0 <+332>:  mov    ds:0x8049ab0,eax             save the eax (pointer return by strdup) into global variable service
0x080486b5 <+337>:  lea    eax,[esp+0x20]               load address of [esp+0x20] (pointer to local variable1) into eax
0x080486b9 <+341>:  mov    edx,eax                      copy the eax (pointer to local variable1) to edx
0x080486bb <+343>:  mov    eax,0x804882d                copy "login" into eax
0x080486c0 <+348>:  mov    ecx,0x5                      copy number 5 into ecx
0x080486c5 <+353>:  mov    esi,edx                      copy edx (pointer to local variable1) into esi (source index)
0x080486c7 <+355>:  mov    edi,eax                      copy eax ("login") into edi (destination index)
0x080486c9 <+357>:  repz   cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
                                                        repeat Compare String Operands (cmps) on esi and edi
0x080486cb <+359>:  seta   dl                           set dl (lower part of dx) to 1 if the above flag is set, or dl to 0 if not
0x080486ce <+362>:  setb   al                           set al (lower part of ax) to 1 if the below flag is set, or al to 0 if not
0x080486d1 <+365>:  mov    ecx,edx                      copy edx (pointer to local variable1) into ecx
0x080486d3 <+367>:  sub    cl,al                        subtract al (lower part of ax) with cl (lower part of cx)
0x080486d5 <+369>:  mov    eax,ecx                      copy ecx (after the lower part gets substracted by al) into eax
0x080486d7 <+371>:  movsx  eax,al movsx is to           copy a smaller memory to bigger memory by sign extension (al is lower part of ax)
0x080486da <+374>:  test   eax,eax                      all those operation above is just to check if the result of the cmp is equal to 0
0x080486dc <+376>:  jne    0x8048574 <main+16>          jump not equal to line main+16 (This looks like while(1) loop, as it goes back to the top without a condition to stop
0x080486e2 <+382>:  mov    eax,ds:0x8049aac             copy address of global variable auth into eax
0x080486e7 <+387>:  mov    eax,DWORD PTR [eax+0x20]     copy the content of [eax+0x20] (global variable auth + 32) into eax
0x080486ea <+390>:  test   eax,eax                      test if eax = 0
0x080486ec <+392>:  je     0x80486ff <main+411>         jump equal to line main+411
0x080486ee <+394>:  mov    DWORD PTR [esp],0x8048833    put "/bin/sh" to the top of the stack, as the 1st arg for the system function
0x080486f5 <+401>:  call   0x8048480 <system@plt>       call function system
0x080486fa <+406>:  jmp    0x8048574 <main+16>          jump not equal to line main+16 (This looks like while(1) loop, as it goes back to the top without a condition to stop
0x080486ff <+411>:  mov    eax,ds:0x8049aa0             put "stdout" into eax
0x08048704 <+416>:  mov    edx,eax                      copy eax (stdout) into edx
0x08048706 <+418>:  mov    eax,0x804883b                copy "Password:\n" into eax
0x0804870b <+423>:  mov    DWORD PTR [esp+0xc],edx      put edx (stdout) to 12bytes below the top of the stack, as the 4arg for the fwrite function
0x0804870f <+427>:  mov    DWORD PTR [esp+0x8],0xa      put number 10 to 8bytes below the top of the stack, as the 3rd arg for the fwrite function
0x08048717 <+435>:  mov    DWORD PTR [esp+0x4],0x1      put number 1 to 4bytes below the top of the stack, as the 2nd arg for the fwrite function
0x0804871f <+443>:  mov    DWORD PTR [esp],eax          put eax ("Password:\n") to the top of the stack, as the 1st arg for the fwrite function
0x08048722 <+446>:  call   0x8048450 <fwrite@plt>       call function fwrite
0x08048727 <+451>:  jmp    0x8048574 <main+16>          jump to line main+16 (This looks like while(1) loop, as it goes back to the top without a condition to stop
0x0804872c <+456>:  nop                                 No operation
0x0804872d <+457>:  mov    eax,0x0                      return 0
0x08048732 <+462>:  lea    esp,[ebp-0x8]  
0x08048735 <+465>:  pop    esi  
0x08048736 <+466>:  pop    edi  
0x08048737 <+467>:  pop    ebp  
0x08048738 <+468>:  ret 
```