# Level 1 reserve asm

<h3>run()</h3>

```console
0x08048444 <+0>:	push   ebp	
0x08048445 <+1>:	mov    ebp,esp	
0x08048447 <+3>:	sub    esp,0x18	                  Buffer 24 bytes for local variable
0x0804844a <+6>:	mov    eax,ds:0x80497c0           ds (data segment) the content is stdout, copy this result to eax
0x0804844f <+11>:	mov    edx,eax	                  copy eax to edx as edx is related to input/output operations
0x08048451 <+13>:	mov    eax,0x8048570	            copy "Good... Wait what?\n" to eax
0x08048456 <+18>:	mov    DWORD PTR [esp+0xc],edx	  put edx (stdout) to 12bytes below the top of the stack, as the 4arg for the fwrite function
0x0804845a <+22>:	mov    DWORD PTR [esp+0x8],0x13	  put number 19 to 8bytes below the top of the stack, as the 3rd arg for the fwrite function
0x08048462 <+30>:	mov    DWORD PTR [esp+0x4],0x1	  put number 1 to 4bytes below the top of the stack, as the 2nd arg for the fwrite function
0x0804846a <+38>:	mov    DWORD PTR [esp],eax	      put eax ("Good... Wait what?\n") to the top of the stack, as the 1st arg for the fwrite function
0x0804846d <+41>:	call   0x8048350 <fwrite@plt>	    call function fwrite
0x08048472 <+46>:	mov    DWORD PTR [esp],0x8048584	put eax ("/bin/sh") to the top of the stack, as the 1st arg for the system function
0x08048479 <+53>:	call   0x8048360 <system@plt>	    call function system
0x0804847e <+58>:	leave	
0x0804847f <+59>:	ret	
```

<h3>main()</h3>

```console
0x08048480 <+0>:	push   ebp	
0x08048481 <+1>:	mov    ebp,esp	
0x08048483 <+3>:	and    esp,0xfffffff0	            Bit alignment
0x08048486 <+6>:	sub    esp,0x50	                  Buffer 80 bytes for local variable
0x08048489 <+9>:	lea    eax,[esp+0x10]	            load address of [esp+0x10] (pointer to local variable1) into eax (since gets expect a char*, we can assume this is have a buffer of 64 (0x50 - 0x10)
0x0804848d <+13>:	mov    DWORD PTR [esp],eax	      put eax (pointer to local variable1) to the top of the stack, as the 1st arg for the gets function
0x08048490 <+16>:	call   0x8048340 <gets@plt>	      call function gets
0x08048495 <+21>:	leave	
0x08048496 <+22>:	ret	
```

