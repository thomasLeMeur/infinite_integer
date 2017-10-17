# infinite_integer

This library provides a C structure and some functions to manipulate infinite integers in many bases (2 <= base <= 36)

Limits :
- The theorical limit is +/-(2 ^ ((sizeof(uintmax_t) * CHAR_BIT) * UINTMAX_T))
	- With 64bits usual computer, this number is +/-(2 ^ 1180591620717411303424)
- Be carreful, there are concrete limits :
	- size uintmax_t overflows : will crash
	- depending on malloc (usually limited to SIZET_MAX) : will crash
		- theorical : +/-(2 ^ SIZET_MAX)
		- usual     : +/-(2 ^ 4294967296)
	- depending on time complexity for very big numbers : will loop
		- add : O(N * 2)
		- sub : O(N * 2)
		- mul : O(N * O(add))
		- pow : O(p * O(mul))
		- div : O(N * O(sub))
		- mod : O(N * O(sub))

Compile :
- The Makefile provides a program to test the C structure and its functions :
	- make re
	- ./infinite_integer_tester
- With a debug global, the Makefile provides the same program with debug informations :
	- INFINITE_INTEGER_DEBUG=1664 make re
	- valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -v ./infinite_integer_tester
	- gcov srcs/*.c (92.47%)
