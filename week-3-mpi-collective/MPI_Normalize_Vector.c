#define _XOPEN_SOURCE

#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

bool VERBOSE = false;
bool CONSTANT_VECTOR = false;
bool PRINT_VECTORS = false;
int MAX_VALUE = 10;
int MIN_VALUE = 0;

//to compile: mpicc -std=c99 -Wall MPI_Normalize_Vector.c -o MPI_Normalize_Vector
//to run (simple): mpirun ./MPI_Normalize_Vector
//to run (multi-process): mpirun -np 4 ./MPI_Normalize_Vector

void print_intVec(int *vec, int size)
{
  int *p;
  int * vec_end = vec+size;
  for (p = vec; p < vec_end; p++)
  {
    printf("%d ", *p);
  }
}

void print_doubleVec(double *vec, int size)
{
  double *p;
  double * vec_end = vec+size;
  for (p = vec; p < vec_end; p++)
  {
    printf("%f ", *p);
  }
}

int * randomize_intVec(int *vec, int size)
{
  int *p;
  int * vec_end = vec+size;
  for (p = vec; p < vec_end; p++)
  {
    if(CONSTANT_VECTOR){
	     //Shout out Ryan Kaplan for this lil bit
       *p = rand() % (100+1 - 100) + 100;
    } else {
       *p = rand() % (MAX_VALUE + 1 - MIN_VALUE) + MIN_VALUE;
    }
  }
  return vec;
}

double * normalize_vector(int *vec, double *result, int size, double magnitude)
{
	int *p;
	double *p2;
	int * vec_end = vec+size;
  	double * result_end = result+size;
  	for (p = vec, p2 = result; (p < vec_end && p2 < result_end); p++, p2++)
  	{
		*p2 = *p / magnitude;
	}
	return result;
}

double parallel_magnitude(double *vector, int size, int my_rank, int comm_sz){
  int sliceSize = size/comm_sz;
	double *slice = malloc(sliceSize*sizeof(double));

	double globalSumOfSquares = 0;
	double sliceSum = 0;
	double magnitude = 0;

  //Send slices to all processes
	MPI_Scatter(vector,sliceSize,MPI_DOUBLE,slice,sliceSize,MPI_DOUBLE,0,MPI_COMM_WORLD);

	//Calculate local sum of squares
	int i;
	for(i=0; i< sliceSize; i++){
		double tmp = slice[i];
		sliceSum += (tmp * tmp);
	}

	//Only non-roots need to send back
	if(my_rank != 0){
		MPI_Send(&sliceSum,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
	} else {
		//Add root process's work
		globalSumOfSquares = globalSumOfSquares + sliceSum;

		//Add local sums from non root nodes
		int src;
		for(src = 1; src < comm_sz; src++){
			double tmp;
			MPI_Recv(&tmp,1,MPI_DOUBLE,src,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			globalSumOfSquares = globalSumOfSquares + tmp;
		}

		//Take square root
		magnitude = sqrt(globalSumOfSquares);
    // printf("Magnitude: %f", magnitude);
	}
  return magnitude;
}

int main(int argc, char *argv[]){


	int        comm_sz;               /* Number of processes    */
	int        my_rank;               /* My process rank        */

	// double *prod_vector;
	// double *my_vector;
	//Don't put any MPI stuff before this line
	MPI_Init(&argc, &argv);  //sets up the MPI. Always this line!

	/* Get the number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	/* Get my rank among all the processes */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	//Setup randomization
	srand48(time(NULL));

	//Collect and interpret command line args
	int SIZE = 0;
	if (argc != 2){
		if(my_rank == 0){
			printf("usage: MPI_vector_dot <size>\n ");
		}
		exit(1);
	}
	else {
		SIZE = atoi(argv[1]);
	}

	if (SIZE%comm_sz != 0){
		if(my_rank == 0){
			printf("Can't do this on %d processes\n",comm_sz);
		}
		exit(1);
	}

	// Start the clock
	double start_time = MPI_Wtime();

	int sliceSize = SIZE/comm_sz;
	int *slice = malloc(sliceSize*sizeof(int));
	double *sliceNorm = malloc(sliceSize*sizeof(double));
	int *bigArray;
	double *bigResult;

	int globalSumOfSquares = 0;
	int sliceSum = 0;

	double magnitude;

	if (my_rank == 0)
	{
		//Make the random vector
		bigArray = malloc(SIZE*sizeof(int));
		bigArray = randomize_intVec(bigArray, SIZE);

    //And the result vector
    bigResult = malloc(SIZE*sizeof(double));
	}

  //Send slices to all processes
	MPI_Scatter(bigArray,sliceSize,MPI_INT,slice,sliceSize,MPI_INT,0,MPI_COMM_WORLD);

	//Calculate local sum of squares
	int i;
	for(i=0; i< sliceSize; i++){
		int tmp = slice[i];
		sliceSum += (tmp * tmp);
	}

	//Only non-roots need to send back
	if(my_rank != 0){
		MPI_Send(&sliceSum,1,MPI_INT,0,0,MPI_COMM_WORLD);
	} else {
		//Add root process's work
		globalSumOfSquares = globalSumOfSquares + sliceSum;

		//Add local sums from non root nodes
		int src;
		for(src = 1; src < comm_sz; src++){
			int tmp;
			MPI_Recv(&tmp,1,MPI_INT,src,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			globalSumOfSquares = globalSumOfSquares + tmp;
		}
		//Take square root
		magnitude = sqrt(globalSumOfSquares);
	}

	//Broadcast magnitude to nodes
	MPI_Bcast(&magnitude,1,MPI_DOUBLE,0,MPI_COMM_WORLD);

	//Calculate local sum of squares (dont divide by zero)
	if(magnitude > 0){
		sliceNorm = normalize_vector(slice, sliceNorm, sliceSize, magnitude);
	}

	MPI_Gather(sliceNorm,sliceSize,MPI_DOUBLE,bigResult,sliceSize,MPI_DOUBLE,0,MPI_COMM_WORLD);

  // Stop the clock
  double end_time = MPI_Wtime();
  double elapsed = end_time-start_time;

  //Test normalization by getting magnitude of the result vector
  double magnitudeOfNormalized = parallel_magnitude(bigResult,SIZE,my_rank, comm_sz);

	//Print stuff
	if(my_rank == 0){
    if(PRINT_VECTORS){
      printf("Main Vector: ");
      print_intVec(bigArray, SIZE);
      printf("\nNormalized: ");
      print_doubleVec(bigResult, SIZE);
    }
		printf("\nGlobal Sum of Squares: %d\nMagnitude of BigArray: %f",globalSumOfSquares, magnitude);
    printf("\nMagnitude of Normalized Vector: %f\n",magnitudeOfNormalized);
    printf("\nThat took %f seconds", elapsed);
	}



	if(VERBOSE){
		printf("\nProcess %d ------>\nSlice: ",my_rank);
		print_intVec(slice, sliceSize);
		printf("\nSliceNorm: ");
		print_doubleVec(sliceNorm, sliceSize);
		printf("\nLocal Sum: %d ",sliceSum);
		printf("\nReceived Magnitude: %f\n",magnitude);
	}

	MPI_Finalize();
	//please no MPI stuff after this line, okay :)
	return 0;
}
