#include <stdio.h>

/* any bit of x equals 1 => if (x) { .. }
 * any bit of x equals 0 => if (!x) { ..}
 * any bit in the least significant byte of x equals 1 => if !!(x & 0xff)
 * any bit in the most significant byte of x equals 0 => if !!(x & 0x000000ff) assuming 32bit int
 * 
 */

int main(int argc, const char *argv[])
{
  return 0;
}
