#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

long double estimate_pi(int N, int nthreads){
  long double dx = 1.0/(long double)N;
  long double sum = 0.0;
  printf("dx: %Le\n", dx);
  #pragma omp parallel for reduction(+:sum) num_threads(nthreads)
  for(int i = 1; i < N; i++){
    long double w, xval, h;
    w = dx;
    xval = i * dx;
    h = sqrt(1-(xval*xval));
    sum += w*h;
  }
  printf("sum: %Le\n", sum);
  return (long double) 4 * sum;
}

long double non_parallel_pi(int N){
  long double dx = 1.0/(long double)N;
  long double sum = 0.0;
  long double w, xval, h;
  for(int i = 1; i < N; i++){
    w = dx;
    xval = i * dx;
    h = sqrt(1-(xval*xval));
    sum += w*h;
  }
  return (long double)4 * sum;
}

int main(int argc, char *argv[])
{
  int N = 10;
  int nthreads = 1;
  if (argc > 4){
    printf("usage: omp-pi <procs> <N>\n ");
    exit(1);
  }
  else {
      if (argc > 1)
        nthreads = strtol(argv[1],NULL,10);
      if (argc > 2)
        N = atoi(argv[2]); //atoi converts a string to an int
  }
  printf("\nthreads: %d, N size: %d\n",nthreads,N);

  double start = omp_get_wtime();
  long double pi = estimate_pi(N, nthreads);
  double end = omp_get_wtime();
  printf("pi = %Le\n", pi);
  printf("Estimating pi took (seconds): %f\n", end-start);

  // long double pi2 = non_parallel_pi(N);
  // printf("p2 = %Le\n", pi2);
}
