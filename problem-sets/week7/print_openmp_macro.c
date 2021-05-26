#include <stdio.h>
#ifdef _OPENMP
  #include <omp.h>
#endif

int main(int argc, char *argv[])
{
    #ifdef _OPENMP
      printf("_OPENMP = %d\n", _OPENMP);
    #endif
}
