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
bool TEST_PART_5 = false;
bool TEST_PART_6 = false;
bool TEST_PART_7 = false;
bool VERBOSE_TESTS = false;
bool TEST_DOT_PRODUCT_TIME = true;
bool CONSTANT_VECTORS = false;

double * fill_vector(double *vec, int size, double filler[])
{
  int index;
  for (index= 0; index< size; index++ ) {
   vec[index] = filler[index];
  }
  return vec;
}

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

double dot_product(double *vec, double *vec2, int size){
  double product = 0;
  double *p ,*p2;
  double * vec_end = vec+size;
  double * vec2_end = vec2+size;
  for (p = vec, p2 = vec2; (p < vec_end && p2 < vec2_end); p++, p2++)
  {
    product = (*p * *p2) + product;
  }
  return product;
}

int main(int argc, char *argv[])
{
  clock_t startTime = clock();

  int SIZE = 0;
  if ((argc < 2) || (argc > 3)){
    printf("usage: vectors <size> or vectors <size> <seed>\n ");
    exit(1);
  }
  else {
      SIZE = atoi(argv[1]); //atoi converts a string to an int
      printf("size: %d",SIZE);
      if (argc == 3)
        srand48(atoi(argv[2]));
      else
        srand48(time(NULL));
  }

  if(TEST_PART_4)
  {
    printf("\nTesting pointer-based randomization\n");
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
    printf("\nTesting vector addition\n");
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

  if(TEST_PART_6)
  {
    printf("\nTesting vector dot product\n");
    double * vector1 =  calloc(SIZE,sizeof(double));
    double * vector2 =  calloc(SIZE,sizeof(double));
    vector1 = randomize_vector(vector1, SIZE);
    vector2 = randomize_vector(vector2, SIZE);
    printf("Vector 1\n");
    print_vector(vector1,SIZE);
    printf("\nVector 2\n");
    print_vector(vector2,SIZE);

    double product =  dot_product(vector1,vector2, SIZE);

    printf("\nDot product is %lf\n", product);
  }

  if(TEST_PART_7)
  {
    printf("\nTesting running time\n");

    clock_t start = clock();
    double * vector1 =  calloc(SIZE,sizeof(double));
    vector1 = randomize_vector(vector1, SIZE);
    clock_t thisTime = clock();
    double elapsedTime =  (double)(thisTime - start) * 1000.0 / CLOCKS_PER_SEC;
    printf("\nrandomize_vector took %f ms\n",elapsedTime);

    start = clock();
    double * vector2 =  calloc(SIZE,sizeof(double));
    vector2 = randomize_vector_p(vector2, SIZE);
    thisTime = clock();
    elapsedTime =  (double)(thisTime - start) * 1000.0 / CLOCKS_PER_SEC;
    printf("\nrandomize_vector_p took %f ms\n",elapsedTime);

    if(VERBOSE_TESTS){
      printf("\n");
      start = clock();
      print_vector(vector1,SIZE);
      thisTime = clock();
      elapsedTime =  (double)(thisTime - start) * 1000.0 / CLOCKS_PER_SEC;
      printf("\n\nprint_vector took %f ms\n\n",elapsedTime);

      start = clock();
      print_vector_p(vector2,SIZE);
      thisTime = clock();
      elapsedTime =  (double)(thisTime - start) * 1000.0 / CLOCKS_PER_SEC;
      printf("\n\nprint_vector_p took %f ms\n",elapsedTime);
    }

    start = clock();
    double * vector3 = add_vectors(vector1,vector2, SIZE);
    thisTime = clock();
    elapsedTime =  (double)(thisTime - start) * 1000.0 / CLOCKS_PER_SEC;
    printf("\nadd_vectors took %f ms\n",elapsedTime);

    start = clock();
    double product =  dot_product(vector1,vector2, SIZE);
    thisTime = clock();
    elapsedTime =  (double)(thisTime - start) * 1000.0 / CLOCKS_PER_SEC;
    printf("\ndot_product took %f ms\n",elapsedTime);
  }

  if(TEST_DOT_PRODUCT_TIME){
    clock_t start = clock();
    double * vector1 =  calloc(SIZE,sizeof(double));
    double * vector2 =  calloc(SIZE,sizeof(double));
    if(!CONSTANT_VECTORS){
			//Randomize using pointer method
			vector1 = randomize_vector_p(vector1, SIZE);
			vector2 = randomize_vector_p(vector2, SIZE);
		} else {
			//Hard-coded test vectors (expected product vec1 • vec2 = 2.776287700675)
			double vec1_array[] = {0.315817,0.021303,0.933138,0.463567,0.606694,
							0.708238,0.638140,0.563823,0.733255,0.401841};
			double vec2_array[] = {0.291766,0.439373,0.233446,0.231762,0.968261,
							0.122894,0.741578,0.943867,0.410790,0.916815};
			vector1 = fill_vector(vector1, SIZE, vec1_array);
			vector2 = fill_vector(vector2, SIZE, vec2_array);
		}

    if(VERBOSE_TESTS){
      printf("Vector1: ");
      print_vector_p(vector1, SIZE);
      printf("\nVector2: ");
      print_vector_p(vector2, SIZE);
    }

    double product =  dot_product(vector1,vector2, SIZE);
    clock_t thisTime = clock();
    printf("\nvector1 • vector2 = %f",product);
    double elapsedTime =  (double)(thisTime - start) * (double)1000.0000 / CLOCKS_PER_SEC;
    printf("\ndot_product took %f ms\n",elapsedTime);
  }

  // clock_t endTime = clock();
  // double elapsed =  (double)(endTime - startTime) * 1000.0 / CLOCKS_PER_SEC;
  // printf("\nDone, that took %f ms\n",elapsed);
}
