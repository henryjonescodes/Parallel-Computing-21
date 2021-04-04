#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

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

bool TEST_PART_4 = false;
bool TEST_PART_5 = true;

double * randomize_vector(double *vec, int size)
{
  int index;
  for (index= 0; index< size; index++ ) {
   vec[index] = drand48();
  }
  return vec;
}

double * randomize_vector_p(double *vec, int size)
{
  double *p;
  double * vec_end = vec+size;
  for (p = vec; p < vec_end; p++)
  {
    *p = drand48();
  }
  return vec;
}

void print_vector(double *vec, int size)
{
  int index;
  for (index= 0; index< size; index++ ) {
    printf("%f ", vec[index]);
  }

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

double * add_vectors(double *vec, double *vec2, int size){
  double * vec3 =  calloc(size,sizeof(double));
  double *p ,*p2, *p3;
  double * vec_end = vec+size;
  double * vec2_end = vec2+size;
  double * vec3_end = vec3+size;
  for (p = vec, p2 = vec2, p3 = vec3; (p < vec_end && p2 < vec2_end && p3 < vec3_end); p++, p2++, p3++)
  {
    *p3 = *p + *p2;
  }
  return vec3;
}


int main(int argc, char *argv[])
{
  int SIZE = 0;

  if ((argc < 2) || (argc > 3)){
    printf("usage: vectors <size> or vectors <size> <seed>\n ");
    exit(1);
  }
  else {
      SIZE = atoi(argv[1]); //atoi converts a string to an int
      printf("size: %d\n",SIZE);
      if (argc == 3)
        srand48(atoi(argv[2]));
      else
        srand48(time(NULL));
  }

  if(TEST_PART_4)
  {
    //calloc, unlike malloc, zeros the allocated memory
    double * vector1 =  calloc(SIZE,sizeof(double));
    double * vector2 =  calloc(SIZE,sizeof(double));
    double * vector3 =  calloc(SIZE,sizeof(double));
    double * vector4 =  calloc(SIZE,sizeof(double));

    vector1 = randomize_vector(vector1, SIZE);
    vector2 = randomize_vector(vector2, SIZE);
    vector3 = randomize_vector_p(vector3, SIZE);
    vector4 = randomize_vector_p(vector4, SIZE);

    printf("Vector 1 (value randomized and printed)\n");
    print_vector(vector1,SIZE);
    printf("\nVector 2 (value randmoized and pointer printed)\n");
    print_vector_p(vector2,SIZE);
    printf("\nVector 3 (pointer randmoized and value printed)\n");
    print_vector(vector3,SIZE);
    printf("\nVector 4 (pointer randmoized and printed)\n");
    print_vector_p(vector4,SIZE);
    printf("\n");
  }

  if(TEST_PART_5)
  {
    double * vector1 =  calloc(SIZE,sizeof(double));
    double * vector2 =  calloc(SIZE,sizeof(double));
    vector1 = randomize_vector(vector1, SIZE);
    vector2 = randomize_vector(vector2, SIZE);

    printf("Vector 1\n");
    print_vector(vector1,SIZE);
    printf("\nVector 2\n");
    print_vector(vector2,SIZE);

    double * vector3 =  add_vectors(vector1,vector2, SIZE);

    printf("\nVector 3 (added vector1 with vector 2)\n");
    print_vector(vector3,SIZE);
    printf("\n");
  }
}
