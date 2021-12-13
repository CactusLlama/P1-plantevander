#include <time.h>
#include <stdlib.h>

int first = 1;

int get_random(int min, int max){
  int result;
  if (first==1)
  {
    srand(time(NULL));
    first = 0;
  }
  result = (rand() % ( max + 1 - min)) + min;

  return result;
}
