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

void print_vector_p(double *vec, int size)
{
  double *p;
  double * vec_end = vec+size;
  for (p = vec; p < vec_end; p++)
  {
    printf("%f ", *p);
  }

}

void vector_add(double *v1, double* v2, double *result, int size, int nthreads)
{
  //write me with openmp
}

//return the mean of a vector
double vector_mean(double *v1, int size, int nthreads)
{
  //write me with openmp
}

//return the magnitude of a a vector
// (squareroot of sum of squares )
double vector_mag(double *v1, int size, int nthreads)
{
  //write me with openmp
}

//normalize a vector, putting result in outv 
void vector_normalize(double *v1, double *outv, int size, int nthreads)
{
 //first a call to vector mag
 //then an omp for loop that divides evertying in the vector by magnitude
 //warning don't ever omp parallelize loops that call functions that also parallelize 
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

print_vector(vector1,SIZE);
print_vector(vector2,SIZE);

printf("%f\n",sqrt(2.0));

}
