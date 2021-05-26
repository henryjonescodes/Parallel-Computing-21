#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

/*
vectors.c for week 1 bootcamp
author: John Rieffel

*/

//argc is the argument count from the commandline
//argc is always at least 1, because the excutable
//is an argument
//
//each argument is held in a separate string.
//argv is an *array* of strings

double * randomize_vector(double *vec, int size)
{
  int index;
  for (index= 0; index< size; index++ ) {
   vec[index] = drand48();
  }
  return vec;
}

void print_vector(double *vec, int size)
{
  int index;
  for (index= 0; index< size; index++ ) {
    printf("%f ", vec[index]);
}
  printf("\n");
  return;
}

void vector_add(double *v1, double* v2, double *result, int size, int nthreads)
{
  #pragma omp parallel num_threads(nthreads)
  {
    int myid = omp_get_thread_num();
  	int threadcount = omp_get_num_threads();
    for (int i = myid ; i < size; i+=threadcount)
  	{
      result[i] = v1[i] + v2[i];
    }
  }
}

//return the mean of a vector
double vector_mean(double *v1, int size, int nthreads)
{
  double globalsum = 0;
  #pragma omp parallel num_threads(nthreads)
  {
    double localsum = 0;
    int myid = omp_get_thread_num();
  	int threadcount = omp_get_num_threads();
    for (int i = myid ; i < size; i+=threadcount)
    {
      localsum += v1[i];
    }
  #pragma omp critical
  globalsum += localsum;
  }
  return globalsum/(double)size;
}

//return the magnitude of a a vector
// (squareroot of sum of squares )
double vector_mag(double *v1, int size, int nthreads)
{
  double avg = vector_mean(v1, size, nthreads);
  double globalss = 0;
  #pragma omp parallel num_threads(nthreads)
  {
    double localss = 0;
    int myid = omp_get_thread_num();
  	int threadcount = omp_get_num_threads();
    for (int i = myid ; i < size; i+=threadcount)
    {
      localss += v1[i] * v1[i];
    }
  #pragma omp critical
  globalss += localss;
  }
  double magnitude = sqrt(globalss);
  return magnitude;
}

//normalize a vector, putting result in outv
void vector_normalize(double *v1, double *outv, int size, int nthreads)
{
  double mag = vector_mag(v1, size, nthreads);
  #pragma omp parallel num_threads(nthreads)
  {
    int myid = omp_get_thread_num();
  	int threadcount = omp_get_num_threads();
    for (int i = myid ; i < size; i+=threadcount)
    {
      outv[i] = v1[i]/mag;
    }
  }
}

int main(int argc, char *argv[])
{
  int SIZE = 10;
  int nthreads = 1;
  if (argc > 4){
    printf("usage: vectors <procs> <size> or vectors <procs> <size> <seed>\n ");
    exit(1);
  }
  else {
      if (argc > 1)
        nthreads = strtol(argv[1],NULL,10);
      if (argc > 2)
        SIZE = atoi(argv[2]); //atoi converts a string to an int
      if (argc > 3)
        srand48(atoi(argv[3]));
      else
        srand48(time(NULL));
  }
  printf("threads: %d, vector size: %d\n",nthreads,SIZE);

//calloc, unlike malloc, zeros the allocated memory
double * vector1 =  calloc(SIZE,sizeof(double));
double * vector2 =  calloc(SIZE,sizeof(double));
double * vector3 =  calloc(SIZE,sizeof(double));

vector1 = randomize_vector(vector1, SIZE);
vector2 = randomize_vector(vector2, SIZE);

printf("\nBefore Process:\n");
print_vector(vector1,SIZE);
// print_vector(vector2,SIZE);

double mean = vector_mean(vector1, SIZE, nthreads);
printf("Mean: %f\n", mean);
double mag = vector_mag(vector1, SIZE, nthreads);
printf("Magnitude: %f\n", mag);

vector_normalize(vector1, vector3, SIZE, nthreads);

printf("After Process:\n");
print_vector(vector3,SIZE);

printf("%f\n",sqrt(2.0));

}
