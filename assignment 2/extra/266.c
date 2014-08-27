#include <stdio.h>

/* Generate mask indicating leftmost 1 in x */

int leftmost_one(unsigned x) {
  x |= x >> 1;
  x |= x >> 2;
  x |= x >> 4;
  x |= x >> 8;
  x |= x >> 16;
  return x - (x >> 1); /* 1111 1111 1111 1111 - 0111 1111 1111 1111 (could also use xor here) */
}

int main(int argc, const char *argv[])
{
  int x = 0x0;
  printf("%x\n", leftmost_one(x));
  return 0;
}
