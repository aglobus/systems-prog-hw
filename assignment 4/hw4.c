#include <stdio.h>
#include <stdlib.h>

/* Assignment #4 Alex Globus */
/* K&R 5-5, 5-9 */
/* B&0 3.54, 3.58, 3.59, 3.66 */


/* K&R 5-5 */
/* Write clones for strncpy, strncat, and strcmp */

char *strncpy_clone(char *, char *, size_t);
char *strncat_clone(char *, char *, size_t);
int strcmp_clone(char *, char *, size_t);

char *strncpy_clone(char *dest, char *src, size_t n)
{
  char *tmp = dest;
  while (n-- && (*dest++ = *src++));
  return tmp;
}

char *strncat_clone(char *dest, char *src, size_t n)
{
  char *tmp = dest;
  /* go to end */
  while (*dest != '\0')
    dest++;

  char c;
  while (n-- && ((c = *src++) != '\0'))
    *dest++ = c;
  *dest++ = '\0';

  return tmp;
}

int strcmp_clone(char *one, char *two, size_t n)
{
  char c1, c2;
  int diff = 0;
  do {
    c1 = *one++;
    c2 = *two++;
    diff = c1 - c2;
  } while (n-- && (diff == 0) && (c1 != '\0'));
  return diff;
}

/* K&R 5-9 */
/* rewrite day_of_year and month_day with pointers instead of indexing */

static char daytab[2][13] = {
  {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
  {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

int day_of_year(int year, int month, int day)
{
  int i, leap;
  leap = (year%4 == 0 && year%100 != 0) || year%400 == 0;
  char *p = &daytab[leap][1];
  for (i = 1; i < month; i++) {
    day += *p;
    ++p;
  }
  return day;
}

void month_day(int year, int yearday, int *pmonth, int *pday)
{
  int i, leap;
  leap = (year%4 == 0 && year%100 != 0) || year%400 == 0;
  char *p = &daytab[leap][1];
  for (i = 1; yearday > *p; i++) {
    yearday -= *p;
    ++p;
  }
  *pmonth = i;
  *pday = yearday;
}



/* B&O 3.54 */
/* 

   A function with prototype
   int decode2(int x, int y, int z);
   is compiled into IA32 assembly code. The body of the code is as follows:

   x at %ebp+8, y at %ebp+12, z at %ebp+16

   1    movl    12(%ebp), %edx
   2    subl    16(%ebp), %edx
   3    movl    %edx, %eax
   4    sall    $31, %eax
   5    sarl    $31, %eax
   6    imull   8(%ebp), %edx
   7    xorl    %edx, %eax

   movl 12(%ebp), %edx  ; move y into edx
   subl 16(%ebp), %edx  ; edx is y-z
   movl %edx, %eax      ; store that result into eax
   sall $31, %eax       ; shift result by 31 (<<)
   sarl $31, %eax       ; do it again but in the other direction
   imull 8(%ebp), %edx  ; multiply edx (y-z) by x
   xorl %edx, %eax      ; raise edx to the power of eax

*/

int decode2(int x, int y, int z) {
  return ((y-z) * x) ^ (((y-z) << 31) >> 31);
}




/* B&O 3.58 */
/*
The code that follows shows an example of branching on an enumerated type value in a switch statement. Recall that enumerated types in C are simply a way to introduce a set of names having associated integer values. By default, the values assigned to the names go from zero upward. In our code, the actions associated with the different case labels have been omitted.

 Enumerated type creates set of constants numbered 0 and upward 
typedef enum {MODE_A, MODE_B, MODE_C, MODE_D, MODE_E} mode_t;
int switch3(int *p1, int *p2, mode_t action)
{
    int result = 0;
    switch(action) {
    case MODE_A:
    case MODE_B:
    case MODE_C:
    case MODE_D:
    case MODE_E:
    default:
}
    return result;
}

The part of the generated assembly code implementing the different actions is shown in Figure 3.43. The annotations indicate the argument locations, the register values, and the case labels for the different jump destinations. Register %edx corresponds to program variable result and is initialized to −1.
Fill in the missing parts of the C code. Watch out for cases that fall through.

Arguments: p1 at %ebp+8, p2 at %ebp+12, action at %ebp+16
Registers: result in %edx (initialized to -1)

The jump targets:

.L17                      ; MODE_E
  movl $17, %edx          ; move 17 into edx
  jmp .L19                ; jump to 
.L13:                     ; MODE_A
  movl 8(%ebp), %eax
  movl (%eax), %edx       ; exx = *p1
  movl 12(%ebp), %ecx
  movl (%ecx), %eax       ; eax = *p2
  movl 8(%ebp), %ecx
  movl %eax, (%ecx)       ; *p1 = *p2
  jmp .L19
.L14:                     ; MODE_B
  movl 12(%ebp), %edx     ; edx = p2
  movl (%edx), %eax       ; eax = *p2
  movl %eax, %edx         ; edx = eax => edx = *p2
  movl 8(%ebp), %ecx
  addl (%ecx), %edx       ; edx = *p1 + *p2 => *p2 += *p1
  movl 12(%ebp), %eax     ; eax = p2
  movl %edx, (%eax)       ; result = *p2 
  jmp .L19
.L15:                     ; MODE_C
  movl 12(%ebp), %edx     
  movl $15, (%edx)        ; *p2 = 15
  movl 8(%ebp), %ecx      ; ecx = p1
  movl (%ecx), %edx       ; edx = *p1 => result = *p1
  jmp .L19
.L16:                     ; MODE_D
  movl 8(%ebp), %edx
  movl (%edx), %eax       ; eax = *p1
  movl 12(%ebp), %ecx
  movl %eax, (%ecx)       ; *p2 = *p1
  movl $17, %edx
.L19:                     ; default
  movl %edx, %eax         ; move edx into the return value


*/

typedef enum {MODE_A, MODE_B, MODE_C, MODE_D, MODE_E} mode_t;

int switch3(int *p1, int *p2, mode_t action)
{
    int result = 0;
    switch(action) {
    case MODE_A:
      result = *p1;
      *p1 = *p2;
    case MODE_B:
      *p2 += *p1;
      result = *p2;
      break;
    case MODE_C:
      *p2 = 15;
      result = *p1;
      break;
    case MODE_D:
      *p2 = *p1;
    case MODE_E:
      result = 17;
      break;
    default:
      result = -1; // %edx corresponds to program variable result is initialized to -1
}
    return result;
}



/* B&O 3.59 */

/*
 This problem will give you a chance to reverse engineer a switch statement from machine code. In the following procedure, the body of the switch statement has been removed:

int switch_prob(int x, int n)
{
  int result = x;
  switch(n) {
    <fill in code here>
  }
  return result;
}
 Figure 3.44 shows the disassembled machine code for the procedure. We can see in lines 4 and 5 that parameters x and n are loaded into registers %eax and %edx, respectively.
The jump table resides in a different area of memory. We can see from the indirect jump on line 9 that the jump table begins at address 0x80485d0. Using the gdb debugger, we can examine the six 4-byte words of memory comprising the jump table with the command x/6w 0x80485d0. gdb prints the following:

(gdb) x/6w 0x80485d0
0x80485d0: 0x08048438 0x08048448 0x08048438 0x0804843d
0x80485e0: 0x08048442 0x08048445

Fill in the body of the switch statement with C code that will have the same behavior as the machine code.    

<switch_prob>:

push %ebp
mov %esp, %ebp
mov 0x8(%ebp), %eax
mov 0xc(%ebp), %edx
sub $0x32, %edx
cmp $0x5, %edx
ja 8048448 <switch_prob+0x28>

8048420:  <switch_prob>:
8048420:  push   %ebp 
8048421:  mov    %esp,%ebp                      ; set up stack frame
8048423:  mov    0x8(%ebp),%eax                 ; move x into eax
8048426:  mov    0xc(%ebp),%edx                 ; move n into edx
8048429:  sub    $0x32,%edx                     ; edx = n - 50 => starts at 50
804842c:  cmp    $0x5,%edx                      ; max is 55
804842f:  ja     8048448 <switch_prob+0x28>     ; jump if above to default
8048431:  jmp    *0x80485d0(,%edx,4)            ; jump in
8048438:  shl    $0x2,%eax                      ; eax <<= 2
804843b:  jmp    804844b <switch_prob+0x2b>     ; break
804843d:  sar    $0x2,%eax                      ; eax >>= 2
8048440:  jmp    804844b <switch_prob+0x2b>     ; break
8048442:  lea    (%eax,%eax,2),%eax             ; eax = 2*eax + eax => eax = eax*3 => eax *= 3
8048445:  imul   %eax,%eax                      ; mul
8048448:  add    $0xa,%eax                      ; eax += 10
804844b:  pop    %ebp
804844c:  ret

*/

int switch_prob(int x, int n)
{
  int result = x;
  switch(n) {
    /* 5 distinct values -> 5 distinct cases in jump table */
    case 50:
    case 52:
      result <<= 2;
      break;
    case 53:
      result >>= 2;
      break;
    case 54:
      result *= 4;
      result *= result;
    default:
      result += 10;
  }
  return result;
}



/* B&O 3.66 */
/*
 You are charged with maintaining a large C program, and you come across the following code:

typedef struct {
    int left;
    a_struct a[CNT];
    int right;
} b_struct;
 
void test(int i, b_struct *bp)
{
  int n = bp->left + bp->right;
  a_struct *ap = &bp->a[i];
  ap->x[ap->idx] = n;
}

00000000 <test>:

push   %ebp
mov    %esp,%ebp              
mov    0x8(%ebp),%eax         ; eax = i
mov    0xc(%ebp),%ecx         ; ecx = bp
lea    (%eax,%eax,4),%eax     ; eax = 4(eax) + eax => 5i
add    0x4(%ecx,%eax,4),%eax  ; eax = 4*5i + bp + 4
mov    0xb8(%ecx),%edx        ; edx = bp + 184
add    (%ecx),%edx            ; edx = bp->right + bp->left 
mov    %edx,0x8(%ecx,%eax,4)  ; 4(eax) + ecx + 8
pop    %ebp
ret

The declarations of the compile-time constant CNT and the structure a_struct are in a file for which you do not have the necessary access privilege. Fortunately, you have a copy of the ‘.o’ version of code, which you are able to disassemble with the objdump program, yielding the disassembly shown in Figure 3.45.
Using your reverse engineering skills, deduce the following.


A. The value of CNT

Since left is the first item in b_struct, bp points to the integer (with size 4). Therefore, bp + 4 points to the first a_struct in array a[CNT]. The offset for each array element in a[i] is addressed with bp + 4 + i(v) with v being the size of each a_struct, the add 0x4(%ecx ,%eax, 4), %eax command shows us that it is 20i + bp + 4, meaning each a_struct is of size 20.

bp->right is accessed with 184+4. Since right is an int and of size 4, the entire size of a[CNT] is 180 (184-4). If the entire size is 180 and each element is of size 20, then 180/20 is CNT = 9.


B. A complete declaration of structure a_struct. Assume that the only fields in this structure are idx and x.

From the C code above, it's clear that the two a_struct members are idx and an array x. n is stored into edx from add (%ecx), %edx, which stores bp->right + bp->left into edx. The last line stores edx into 4*eax + ecx + 8, where bp + 8 points to idx and 4(bp + 4 + 20i) reaches into a[i], or, *ap.
So, we can deduce that the a_struct is:

typedef struct {
  int x[4];
  int idx;
} a_struct;
*/




/*
Additional problem
 Consider the following assembly representation of a function foo containing a for loop:

foo:
   pushl %ebp
   movl %esp,%ebp
   pushl %ebx
   movl 8(%ebp),%ebx          ; ebx = a
   leal 2(%ebx),%edx          ; edx = a + 2
   xorl %ecx,%ecx             ; ecx = 0
   cmpl %ebx,%ecx             ; compare a with ecx (i) 
   jge .L4                    ; jump if greater or equal
.L6:
   leal 5(%ecx,%edx),%edx     ; edx = edx + ecx + 5
   leal 3(%ecx),%eax          ; eax = ecx + 3
   imull %eax,%edx            ; edx = edx * eax => (result * (ecx + 3))
   incl %ecx                  ; increment i
   cmpl %ebx,%ecx             ; compare 
   jl .L6                     ; go back to the top
.L4:
   movl %edx,%eax             ; eax = edx
   popl %ebx                  ; return/stack stuff, eax has return value (result var)
   movl %ebp,%esp
   popl %ebp
   ret

Based on the assembly code above, fill in the blanks below in its corresponding C source code.
To ensure full credit, explain your analysis of the assembly code.
Note: you may only use the symbolic variables a, i, and result in your expressions below. Do not use register names.

int foo(int a)
{
    int i;
    int result = _____________;

//for loop here

    return result;
}
*/

int foo(int a)
{
  int i;
  int result = a + 2;

  for (i = 0; i < a; i++) {
    result = result + i + 5;
    result = result * (i + 3);
  }

  return result;
}
