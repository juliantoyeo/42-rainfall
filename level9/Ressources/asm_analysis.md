# Level 9 asm analysis

```console
_ZN1NC2Ei() -> N:N(int)                                         example: l_var1 = new(108), N(l_var1, 5)
0x080486f6 <+0>:    push   ebp  
0x080486f7 <+1>:    mov    ebp,esp  
0x080486f9 <+3>:    mov    eax,DWORD PTR [ebp+0x8]              copy [ebp+0x8] (1st arg) into eax -> 1st arg should be the ebx that was created via _Znwj
0x080486fc <+6>:    mov    DWORD PTR [eax],0x8048848            copy 0x8048848, (_ZTV1N+8 or vtable for N 8) into *eax -> save the data of class N into eax, effectively turn it into N class
0x08048702 <+12>:   mov    eax,DWORD PTR [ebp+0x8]              copy [ebp+0x8] (1st arg) into eax -> get the address of the 1st arg again
0x08048705 <+15>:   mov    edx,DWORD PTR [ebp+0xc]              copy [ebp+0xc] (2nd arg) into edx -> get the 2nd arg passed into the constructor
0x08048708 <+18>:   mov    DWORD PTR [eax+0x68],edx             copy edx (2nd arg) into [eax+0x68] -> saving the 2nd arg into the memory location 0x68 (104) inside N class
                                                                1st arg was allocated 108, so position it looks like the primary private variable for N class
0x0804870b <+21>:   pop    ebp  
0x0804870c <+22>:   ret                                         result in c++: N *l_var1 = new N(5)
    
    
_ZN1N13setAnnotationEPc() -> N::setAnnotation(char*)            example: setAnnotation(l_var1, argv[1])
0x0804870e <+0>:    push   ebp  
0x0804870f <+1>:    mov    ebp,esp  
0x08048711 <+3>:    sub    esp,0x18                             Buffer 24 bytes for local variable
0x08048714 <+6>:    mov    eax,DWORD PTR [ebp+0xc]              copy [ebp+0xc] (2nd arg) into eax -> eax = argv[1]
0x08048717 <+9>:    mov    DWORD PTR [esp],eax                  put eax (argv[1]) to the top of the stack, as the 1st arg for the strlen function
0x0804871a <+12>:   call   0x8048520 <strlen@plt>               call function strlen
0x0804871f <+17>:   mov    edx,DWORD PTR [ebp+0x8]              copy [ebp+0x8] (1st arg) into edx -> edx = l_var1
0x08048722 <+20>:   add    edx,0x4                              add 4 bytes into edx (1st arg + 4) - edx = l_var1 + 4
0x08048725 <+23>:   mov    DWORD PTR [esp+0x8],eax              put eax (return from strlen) to 8bytes below the top of the stack, as the 3rd arg for the memcpy function
0x08048729 <+27>:   mov    eax,DWORD PTR [ebp+0xc]              copy [ebp+0xc] (2nd arg) into eax -> eax = argv[1]
0x0804872c <+30>:   mov    DWORD PTR [esp+0x4],eax              put eax (argv[1]) to 4bytes below the top of the stack, as the 2nd arg for the memcpy function
0x08048730 <+34>:   mov    DWORD PTR [esp],edx                  put edx (l_var1 + 4) to the top of the stack, as the 1st arg for the memcpy function
0x08048733 <+37>:   call   0x8048510 <memcpy@plt>               call function memcpy
0x08048738 <+42>:   leave 
0x08048739 <+43>:   ret                                         result: memcpy(l_var1 + 4, argv[1], strlen(argv[1])
                                                                base the fact that every N class get 108 memory from the new(), and on the constructor function _ZN1NC2Ei (N::N(int))
                                                                we can see that the memory location 0x68 (104) have being used by a private int variable, means that memory 104-108 is being occupied by this int
                                                                that left the memory from 0-103 which could contain some big buffer and 1st arg of memcpy expect a pointer to the dest
                                                                we could assume that there is a char str[100] which is accesible via l_var1 + 4
                                                                Final code will be : memcpy(this.str, argv[1], strlen(argv[1])
    
    
_ZN1NplERS_() -> N::operator+(N&)   
0x0804873a <+0>:    push   ebp  
0x0804873b <+1>:    mov    ebp,esp  
0x0804873d <+3>:    mov    eax,DWORD PTR [ebp+0x8]              copy [ebp+0x8] (1st arg) into eax -> base on the next line we can know that the 1st arg will be a N class
0x08048740 <+6>:    mov    edx,DWORD PTR [eax+0x68]             copy [eax+0x68] (1st arg + 104) into edx -> we can see this is using the private int var inside N class
0x08048743 <+9>:    mov    eax,DWORD PTR [ebp+0xc]              copy [ebp+0xc] (2nd arg) into eax -> base on the next line we can know that the 2nd arg will be a N class
0x08048746 <+12>:   mov    eax,DWORD PTR [eax+0x68]             copy [eax+0x68] (2st arg + 104) into eax -> we can see this is using the private int var inside N class
0x08048749 <+15>:   add    eax,edx                              add eax with edx -> arg1->nb + arg2->nb
0x0804874b <+17>:   pop    ebp  
0x0804874c <+18>:   ret                                         operator+ is operator overloading in c++, which will be to write how '+' should behave
                                                                normally the 1st arg will be the class own variable, and 2nd arg will be the other variable as we can see from the function prototype
    
    
_ZN1NmiERS_() -> N::operator-(N&)   
0x0804874e <+0>:    push   ebp  
0x0804874f <+1>:    mov    ebp,esp  
0x08048751 <+3>:    mov    eax,DWORD PTR [ebp+0x8]              copy [ebp+0x8] (1st arg) into eax -> base on the next line we can know that the 1st arg will be a N class
0x08048754 <+6>:    mov    edx,DWORD PTR [eax+0x68]             copy [eax+0x68] (1st arg + 104) into edx -> we can see this is using the private int var inside N class
0x08048757 <+9>:    mov    eax,DWORD PTR [ebp+0xc]              copy [ebp+0xc] (2nd arg) into eax -> base on the next line we can know that the 2nd arg will be a N class
0x0804875a <+12>:   mov    eax,DWORD PTR [eax+0x68]             copy [eax+0x68] (2st arg + 104) into eax -> we can see this is using the private int var inside N class
0x0804875d <+15>:   mov    ecx,edx                              copy edx (argv1->nb) into ecx
0x0804875f <+17>:   sub    ecx,eax                              sub ecx with eax -> arg1->nb - arg2->nb
0x08048761 <+19>:   mov    eax,ecx                              copy ecx (the sub result) into eax
0x08048763 <+21>:   pop    ebp  
0x08048764 <+22>:   ret                                         same as operator-
    
    
main()    
0x080485f4 <+0>:    push   ebp  
0x080485f5 <+1>:    mov    ebp,esp  
0x080485f7 <+3>:    push   ebx  
0x080485f8 <+4>:    and    esp,0xfffffff0                       Bit alignment
0x080485fb <+7>:    sub    esp,0x20                             Buffer 32 bytes for local variable
0x080485fe <+10>:   cmp    DWORD PTR [ebp+0x8],0x1              compare [ebp+0x8] (argc) to number 1
0x08048602 <+14>:   jg     0x8048610 <main+28>                  jump if greater than 1 to line main+28
0x08048604 <+16>:   mov    DWORD PTR [esp],0x1                  put number 1 to the top of the stack, as the 1st arg for the _exit function
0x0804860b <+23>:   call   0x80484f0 <_exit@plt>                call function _exit
    
0x08048610 <+28>:   mov    DWORD PTR [esp],0x6c                 put number 108 to the top of the stack, as the 1st arg for the _Znwj function
0x08048617 <+35>:   call   0x8048530 <_Znwj@plt>                call function _Znwj (this is the 'new' operator in c++, equivalent to malloc, so it is possibly reserving 108 memory) 
0x0804861c <+40>:   mov    ebx,eax                              copy eax (pointer return by _Znwj) into ebx
0x0804861e <+42>:   mov    DWORD PTR [esp+0x4],0x5              put number 5 to 4bytes below the top of the stack, as the 2nd arg for the _ZN1NC2Ei function
0x08048626 <+50>:   mov    DWORD PTR [esp],ebx                  put ebx (pointer return by _Znwj) to the top of the stack, as the 1st arg for the _ZN1NC2Ei function
0x08048629 <+53>:   call   0x80486f6 <_ZN1NC2Ei>                call function _ZN1NC2Ei
0x0804862e <+58>:   mov    DWORD PTR [esp+0x1c],ebx             copy ebx (that have being processed by _ZN1NC2Ei) into [esp+0x1c] (local variable 1)
                                                                From line <28-58> should looks like this base on the asm code l_var1 = new(108), N(l_var1, 5)
                                                                But to convert it to c++, it will be a little different
                                                                _ZN1NC2Ei is a c++ method that goes throught name mangling, which will be translated to N::N(int)
                                                                From that we could know that there is a class called N, which have a method N(int) which should be the constructor for the class
                                                                In short, all those line above would be equivalent to N *l_var1 = new N(5) in c++
0x08048632 <+62>:   mov    DWORD PTR [esp],0x6c 
0x08048639 <+69>:   call   0x8048530 <_Znwj@plt>  
0x0804863e <+74>:   mov    ebx,eax  
0x08048640 <+76>:   mov    DWORD PTR [esp+0x4],0x6  
0x08048648 <+84>:   mov    DWORD PTR [esp],ebx  
0x0804864b <+87>:   call   0x80486f6 <_ZN1NC2Ei>  
0x08048650 <+92>:   mov    DWORD PTR [esp+0x18],ebx             From line <62-92> is the same as above, but save the class is saved into l_var2
    
0x08048654 <+96>:   mov    eax,DWORD PTR [esp+0x1c]             copy [esp+0x1c] (l_var1) into eax
0x08048658 <+100>:  mov    DWORD PTR [esp+0x14],eax             put eax (l_var1) to 20bytes below the top of the stack
0x0804865c <+104>:  mov    eax,DWORD PTR [esp+0x18]             copy [esp+0x18] (l_var2) into eax
0x08048660 <+108>:  mov    DWORD PTR [esp+0x10],eax             put eax (l_var2) to 16bytes below the top of the stack
0x08048664 <+112>:  mov    eax,DWORD PTR [ebp+0xc]              copy [ebp+0xc] (pointer to argv) into eax
0x08048667 <+115>:  add    eax,0x4                              add 4 bytes into eax (pointer to argv) to get argv[1]
0x0804866a <+118>:  mov    eax,DWORD PTR [eax]                  copy the content in eax (argv[1]) into eax
0x0804866c <+120>:  mov    DWORD PTR [esp+0x4],eax              put eax (argv[1]) to 4bytes below the top of the stack, as the 2nd arg for the _ZN1N13setAnnotationEPc function
0x08048670 <+124>:  mov    eax,DWORD PTR [esp+0x14]             copy [esp+0x14] (l_var1) into eax
0x08048674 <+128>:  mov    DWORD PTR [esp],eax                  put eax (l_var1) to the top of the stack, as the 1st for the _ZN1N13setAnnotationEPc function
0x08048677 <+131>:  call   0x804870e <_ZN1N13setAnnotationEPc>  call function _ZN1N13setAnnotationEPc
                                                                _ZN1N13setAnnotationEPc - > N::setAnnotation(char*)
                                                                It seems like another method inside the N class
                                                                Base on the asm code, it seems like the code should be setAnnotation(l_var1, argv[1])
                                                                But in c++, it would looks like this l_var1->setAnnotation(argv[1])
    
0x0804867c <+136>:  mov    eax,DWORD PTR [esp+0x10]             copy [esp+0x10] (l_var2) into eax
0x08048680 <+140>:  mov    eax,DWORD PTR [eax]                  copy the content of eax (l_var2) into eax -> *l_var2
0x08048682 <+142>:  mov    edx,DWORD PTR [eax]                  copy the content of eax (*l_var2) into edx -> **l_var2
0x08048684 <+144>:  mov    eax,DWORD PTR [esp+0x14]             copy [esp+0x14] (l_var1) into eax
0x08048688 <+148>:  mov    DWORD PTR [esp+0x4],eax              put eax (l_var1) to 4bytes below the top of the stack, for 2nd arg for the function pointed by edx
0x0804868c <+152>:  mov    eax,DWORD PTR [esp+0x10]             copy [esp+0x10] (l_var2) into eax
0x08048690 <+156>:  mov    DWORD PTR [esp],eax                  put eax (l_var2) to the top of the stack, for 1st arg for the function pointed by edx
0x08048693 <+159>:  call   edx                                  call function edx
0x08048695 <+161>:  mov    ebx,DWORD PTR [ebp-0x4]              copy [ebp-0x4] into ebx
0x08048698 <+164>:  leave 
0x08048699 <+165>:  ret 
```