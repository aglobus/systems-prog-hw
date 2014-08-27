#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

void shuffle(char **, int);

/*
 * DESCRIPTION
     These interfaces are obsoleted by arc4random(3).
 */
void shuffle(char **to_shuffle, int size)
{
  /*srand(time(NULL));*/
  struct timeval seed;
  gettimeofday(&seed, NULL);
  srand(seed.tv_usec);
  int i;

  //size refers to the back of the array where we'll put our random swaps
  while (size) {
    i = rand() % size--;
    char *temp;
    temp = to_shuffle[size];
    to_shuffle[size] = to_shuffle[i];
    to_shuffle[i] = temp;
  }
  // or
  /*
   *for (i = size - 1; i > 0; i--) {
   *  int j = rand() % (i + 1);
   *  char *temp;
   *  [> swap <]
   *  temp = to_shuffle[i];
   *  to_shuffle[i] = to_shuffle[j];
   *  to_shuffle[j] = temp;
   *}
   */
}

int main(int argc, const char *argv[])
{
  char *words[] = {"The", "quick", "brown", "fox", "jumped", "over", "the", "lazy", "dog"};
  shuffle(words, 9);

  printf("%s\n\n", "This is a game that tests typing speed");
  printf("%s\n", "Type in the following words:");

  struct timeval start, finish, result;
  gettimeofday(&start, NULL);
  int i;
  for (i = 0; i < 9; i++) {
    char input[20];
    do {
      printf("word #%d is %s: ", i+1, words[i]);
      scanf("%20s", input);
    } while (strcmp(input, words[i]) != 0);
  }
  gettimeofday(&finish, NULL);

  /* calculate the difference between start and finish */
  timersub(&finish, &start, &result);
  /* total time is the sum of tv_sev and tv_usec */
  printf("Correct! Your time is: %d sec %d usec\n", (int)result.tv_sec, (int)result.tv_usec);
  return 0;
}
