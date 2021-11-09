# Bonus 1 asm analysis

```console
0x08048424 <+0>:	  push   ebp	
0x08048425 <+1>:	  mov    ebp,esp	
0x08048427 <+3>:	  and    esp,0xfffffff0	                  Bit alignment
0x0804842a <+6>:	  sub    esp,0x40	                        Buffer 64 bytes for local variable
0x0804842d <+9>:	  mov    eax,DWORD PTR [ebp+0xc]	        copy [ebp+0xc] (pointer to argv) into eax
0x08048430 <+12>:	  add    eax,0x4	                        add 4 bytes into eax (pointer to argv) to get argv[1]
0x08048433 <+15>:	  mov    eax,DWORD PTR [eax]	            copy the content in eax (argv[1]) into eax
0x08048435 <+17>:	  mov    DWORD PTR [esp],eax	            put eax (argv[1]) to the top of the stack, as the 1st arg for the atoi function
0x08048438 <+20>:	  call   0x8048360 <atoi@plt>	            call function atoi
0x0804843d <+25>:	  mov    DWORD PTR [esp+0x3c],eax	        save eax (the result of atoi) to [esp+0x3c] (local variable1)
0x08048441 <+29>:	  cmp    DWORD PTR [esp+0x3c],0x9	        compare [esp+0x3c] (local variable1) with number 9
0x08048446 <+34>:	  jle    0x804844f <main+43>	            jump if less or equal to line main+43
0x08048448 <+36>:	  mov    eax,0x1	                        return 1
0x0804844d <+41>:	  jmp    0x80484a3 <main+127>	            jump to line main+127 which end the program
0x0804844f <+43>:	  mov    eax,DWORD PTR [esp+0x3c]	        copy [esp+0x3c] (local variable1) into eax
0x08048453 <+47>:	  lea    ecx,[eax*4+0x0]	                load address [eax*4+0x0] (local variable1 * 4) into ecx
0x0804845a <+54>:	  mov    eax,DWORD PTR [ebp+0xc]	        copy [ebp+0xc] (pointer to argv) into eax
0x0804845d <+57>:	  add    eax,0x8	                        add 8 bytes into eax (pointer to argv) to get argv[2]
0x08048460 <+60>:	  mov    eax,DWORD PTR [eax]	            copy the content in eax (argv[2]) into eax
0x08048462 <+62>:	  mov    edx,eax	                        copy eax (argv[2]) into edx
0x08048464 <+64>:	  lea    eax,[esp+0x14]	                  load address [esp+0x14] (local variable2) into eax (should be a buffer with size 40bytes (0x3c - 0x14)
0x08048468 <+68>:	  mov    DWORD PTR [esp+0x8],ecx	        put ecx (l_var1 * 4) to 8bytes below the top of the stack, for 3rd arg for the memcpy function
0x0804846c <+72>:	  mov    DWORD PTR [esp+0x4],edx	        put eax (argv[2]) to 4bytes below the top of the stack, for 2nd arg for the memcpy function
0x08048470 <+76>:	  mov    DWORD PTR [esp],eax	            put eax (l_var2) to the top of the stack, as the 1st arg for the memcpy function
0x08048473 <+79>:	  call   0x8048320 <memcpy@plt>	          call function memcpy
0x08048478 <+84>:	  cmp    DWORD PTR [esp+0x3c],0x574f4c46	compare [esp+0x3c] (local variable1) with number 1464814662
0x08048480 <+92>:	  jne    0x804849e <main+122>	            jump not equal to line main+122 (end program)
0x08048482 <+94>:	  mov    DWORD PTR [esp+0x8],0x0	        put number 0 to 8bytes below the top of the stack, for 3rd arg for the execl function
0x0804848a <+102>:	mov    DWORD PTR [esp+0x4],0x8048580	  put "sh" to 4bytes below the top of the stack, for 2nd arg for the execl function
0x08048492 <+110>:	mov    DWORD PTR [esp],0x8048583	      put "/bin/sh" to the top of the stack, as the 1st arg for the memcpy function
0x08048499 <+117>:	call   0x8048350 <execl@plt>	          call function execl
0x0804849e <+122>:	mov    eax,0x0	                        return 0
0x080484a3 <+127>:	leave	
0x080484a4 <+128>:	ret	
```