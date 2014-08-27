#include <stdio.h>

/* answer : (x & 0xff) | (y & ~0xff) */

int main(int argc, const char *argv[])
{
  unsigned int x = 0x89ABCDEF, y = 0x76543210, result;
  result = (x & 0xff) | (y & ~0xff);
  printf("%x\n", result);
  return 0;
}
