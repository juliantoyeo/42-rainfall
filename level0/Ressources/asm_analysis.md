# Level 0 reserve asm

<h3>main()</h3>

```console
0x08048ec0 <+0>:    push   ebp  
0x08048ec1 <+1>:    mov    ebp,esp  
0x08048ec3 <+3>:    and    esp,0xfffffff0             Bit alignment
0x08048ec6 <+6>:    sub    esp,0x20                   Buffer 32 bytes for local variable
0x08048ec9 <+9>:    mov    eax,DWORD PTR [ebp+0xc]    ebp+0xc is the start of argv, now eax store the address of argv[0]
0x08048ecc <+12>:   add    eax,0x4                    eax+0x4 -> address of argv[0] + 4 = address of argv[1]
0x08048ecf <+15>:   mov    eax,DWORD PTR [eax]        copy content of argv[1] base on the address stored in eax
0x08048ed1 <+17>:   mov    DWORD PTR [esp],eax        put eax(content of argv[1]) to esp, which will be the top of the stack, this will become the arg for the function atoi
0x08048ed4 <+20>:   call   0x8049710 <atoi>           call function atoi with the content of argv[1]
0x08048ed9 <+25>:   cmp    eax,0x1a7                  now eax containt the return from atoi, and compare it with 0x1a7 (423)
0x08048ede <+30>:   jne    0x8048f58 <main+152>       jne (jump not equal) to line main+152
0x08048ee0 <+32>:   mov    DWORD PTR [esp],0x80c5348  put "/bin/sh" to the top of the stack (esp)
0x08048ee7 <+39>:   call   0x8050bf0 <strdup>         call function strdup
0x08048eec <+44>:   mov    DWORD PTR [esp+0x10],eax   copy the return of strdup (eax) to [esp+0x10] (local variable1)
0x08048ef0 <+48>:   mov    DWORD PTR [esp+0x14],0x0   copy number 0 to [esp+0x14] (local variable2)
0x08048ef8 <+56>:   call   0x8054680 <getegid>        call function getegid
0x08048efd <+61>:   mov    DWORD PTR [esp+0x1c],eax   copy the return of getegid (eax) to [esp+0x1c] (local variable3)
0x08048f01 <+65>:   call   0x8054670 <geteuid>        call function geteuid
0x08048f06 <+70>:   mov    DWORD PTR [esp+0x18],eax   copy the return of geteuid (eax) to [esp+0x18] (local variable4)
0x08048f0a <+74>:   mov    eax,DWORD PTR [esp+0x1c]   copy [esp+0x1c] (local variable3) to eax
0x08048f0e <+78>:   mov    DWORD PTR [esp+0x8],eax    put eax (local variable3) to 8bytes below the top of the stack, as the 3rd arg for the setresgid function
0x08048f12 <+82>:   mov    eax,DWORD PTR [esp+0x1c]   copy [esp+0x1c] (local variable3) to eax
0x08048f16 <+86>:   mov    DWORD PTR [esp+0x4],eax    put eax (local variable3) to 4bytes below the top of the stack, as the 2nd arg for the setresgid function
0x08048f1a <+90>:   mov    eax,DWORD PTR [esp+0x1c]   copy [esp+0x1c] (local variable3) to eax
0x08048f1e <+94>:   mov    DWORD PTR [esp],eax        put eax (local variable3) to the top of the stack, as the 1st arg for the setresgid function
0x08048f21 <+97>:   call   0x8054700 <setresgid>      call function setresgid
0x08048f26 <+102>:  mov    eax,DWORD PTR [esp+0x18]   copy [esp+0x18] (local variable4) to eax
0x08048f2a <+106>:  mov    DWORD PTR [esp+0x8],eax    put eax (local variable4) to 8bytes below the top of the stack, as the 3rd arg for the setresuid function
0x08048f2e <+110>:  mov    eax,DWORD PTR [esp+0x18]   copy [esp+0x18] (local variable4) to eax
0x08048f32 <+114>:  mov    DWORD PTR [esp+0x4],eax    put eax (local variable4) to 4bytes below the top of the stack, as the 2nd arg for the setresuid function
0x08048f36 <+118>:  mov    eax,DWORD PTR [esp+0x18]   copy [esp+0x18] (local variable4) to eax
0x08048f3a <+122>:  mov    DWORD PTR [esp],eax        put eax (local variable4) to the top of the stack, as the 1st arg for the setresuid function
0x08048f3d <+125>:  call   0x8054690 <setresuid>      call function setresuid
0x08048f42 <+130>:  lea    eax,[esp+0x10]             load address of [esp+0x10] (pointer to local variable1) into eax
0x08048f46 <+134>:  mov    DWORD PTR [esp+0x4],eax    put eax (pointer to local variable1) to 4bytes below the top of the stack, as the 2nd arg for the execv function
0x08048f4a <+138>:  mov    DWORD PTR [esp],0x80c5348  put "/bin/sh" to the top of the stack, as the 1st arg for the execv function
0x08048f51 <+145>:  call   0x8054640 <execv>          call function execv
0x08048f56 <+150>:  jmp    0x8048f80 <main+192>       jump to main+192
0x08048f58 <+152>:  mov    eax,ds:0x80ee170           ds (data segment) i'm not really sure what it does, but the content is stderr, copy this result to eax
0x08048f5d <+157>:  mov    edx,eax                    copy the stderr from eax to edx as edx is related to input/output operations
0x08048f5f <+159>:  mov    eax,0x80c5350              copy "No !\n" to eax
0x08048f64 <+164>:  mov    DWORD PTR [esp+0xc],edx    put edx (stderr) to 12bytes below the top of the stack, as the 4arg for the fwrite function
0x08048f68 <+168>:  mov    DWORD PTR [esp+0x8],0x5    put number 5 to 8bytes below the top of the stack, as the 3rd arg for the fwrite function
0x08048f70 <+176>:  mov    DWORD PTR [esp+0x4],0x1    put number 1 to 4bytes below the top of the stack, as the 2nd arg for the fwrite function
0x08048f78 <+184>:  mov    DWORD PTR [esp],eax        put eax ("No !\n") to the top of the stack, as the 1st arg for the fwrite function
0x08048f7b <+187>:  call   0x804a230 <fwrite>         call functon fwrite
0x08048f80 <+192>:  mov    eax,0x0                    return (0)
0x08048f85 <+197>:  leave 
0x08048f86 <+198>:  ret 
```