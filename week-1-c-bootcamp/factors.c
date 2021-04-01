#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <limits.h>

/*

*/ 


int main(int argc, char *argv[])
{

  //printf ("sizeof unsigned long: %lu",sizeof(unsigned long));
  //printf ("sizeof unsigned long long: %lu",sizeof(unsigned long long));
  unsigned long long bignumber = 4185633689926419097;
  unsigned long i;
  bool isPrime = true;

  clock_t startTime = clock();
  if(bignumber % 2 == 0){
    printf("I found %llu\n",bignumber/2);
  }
  else
  {
    for (i = 3; i < ULLONG_MAX; i++)
    {
      if ((bignumber % i) == 0)
      {
        if (bignumber != i)
        {
          printf("I found %llu\n",bignumber/i);
          isPrime = false;
        }
        i = ULLONG_MAX - 1;
      }
    }
    if(isPrime)
    {
      printf("%llu is prime\n", bignumber);
    }
  }
  clock_t endTime = clock();
  double elapsed =  (double)(endTime - startTime) * 1000.0 / CLOCKS_PER_SEC;
  printf("that took %f ms\n",elapsed);
}
