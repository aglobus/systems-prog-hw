void p2(void);

int main()
{
  p2();
  return 0;
}

/* When this program is compiled and executed on a Linux system, it prints the string “0x55\n” and terminates normally, even though p2 never initializes variable main. Can you explain this? */

/*
 * There can't be multiple definitions of the main symbol. Here, the linker is resolving the symbol in bar6 to the main() function in foo6.
 * If it's true, then the first byte at the memory location of main should be 0x55.

merlin% lldb a.out
Current executable set to 'a.out' (x86_64).
(lldb) x/x main
0x100000f40: 0xe5894855

merlin% otool -tvj a.out | grep -A1 main
_main:
0000000100000f40	55              	pushq	%rbp
*/   
