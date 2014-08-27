#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char *argv[])
{
  if (argc != 2) {
    printf("%s\n", "pass the filename as the argument");  
    return 1;
  }

  /* read file into memory */
  FILE *fp;
  int *buffer;
  long size;

  /* try to open */
  fp = fopen(argv[1], "r");

  /* return if doesn't exist */
  if (!fp)
    return 1;

  /* get the size */
  fseek(fp, 0L, SEEK_END);
  size = ftell(fp);
  fseek(fp, 0L, SEEK_SET); //reset back to beginning

  /* allocate memory we need to store the contents */
  buffer = (int *)calloc(size, sizeof(int));

  if (!buffer) //something went wrong
    return 1;

  fread(buffer, sizeof(int), size, fp);
  fclose(fp);

  /* buffer holds the contents of file */

  /* first stage */
  /* 'a' -> swap bytes c0 and c3 <c0, c1, c2, c3> */
  /* c4 = c0 xor c3 => result = <c0, c1, c2, c3> xor <c4, 0, 0, c4> */

  int i = 0;
  for (; i < size/sizeof(int); i++)
    *(buffer+i) = (*(buffer+i) & 0x00ffff00) | ((*(buffer+i) << 24) | (*(buffer+i) >> 24));

  /* second stage */
  /* 'g' -> swap bits b5 and b3, swap bits b4 and b2 */

  for (i = 0; i < size; i++) {
    *((char *)buffer + i) = (*((char *)buffer+i) & 0xc3) | ((*((char *)buffer+i) >> 2) & 0xc) | ((*((char *)buffer+i) << 2) & 0x30);
  }

  /* third stage */
  /* for every 4 bytes <c0, c1, c2, c3>, XOR c0 and c2 with 'l' */
  for (i = 0; i < size/sizeof(int); i++)
    *(buffer+i) = ((*(buffer+i) & 0x00ff00ff) ^ 0x006c006c) | (*(buffer+i) & 0xff00ff00);

  for (i = 0; i < size; i++)
    printf("%c", *((char *)buffer+i));

  free(buffer);

  return 0;
}
