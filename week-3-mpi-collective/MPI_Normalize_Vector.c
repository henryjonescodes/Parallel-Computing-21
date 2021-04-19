#define _XOPEN_SOURCE

#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

bool VERBOSE = false;

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
	//Shout out Ryan Kaplan for this lil bit
    *p = rand() % (100+1 - 100) + 100;
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

	//Start the clock
	// double start_time = MPI_Wtime();

	int sliceSize = SIZE/comm_sz;
	int *slice = malloc(sliceSize*sizeof(int));
	double *sliceNorm = malloc(sliceSize*sizeof(double));
	int *bigArray;
	double *bigResult;

	int globalSumOfSquares = 0;
	int sliceSum = 0;

	double globalSqrt;

	if (my_rank == 0)
	{
		//Make the random vector
		bigArray = malloc(SIZE*sizeof(int));
		bigArray = randomize_intVec(bigArray, SIZE);
	}
		
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

		//Add local sums
		int src;
		for(src = 1; src < comm_sz; src++){
			int tmp;
			MPI_Recv(&tmp,1,MPI_INT,src,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			globalSumOfSquares = globalSumOfSquares + tmp;
		}

		//Take square root
		globalSqrt = sqrt(globalSumOfSquares);
	}

	//Broadcast result so far to all nodes
	MPI_Bcast(&globalSqrt,1,MPI_DOUBLE,0,MPI_COMM_WORLD);

	//Calculate local sum of squares (dont divide by zero)
	if(globalSqrt > 0){
		sliceNorm = normalize_vector(slice, sliceNorm, sliceSize, globalSqrt);
	}
	if(my_rank == 0){
		bigResult = malloc(SIZE*sizeof(double));
	}
	
	MPI_Gather(sliceNorm,sliceSize,MPI_DOUBLE,bigResult,sliceSize,MPI_DOUBLE,0,MPI_COMM_WORLD);

	//Print stuff
	if(my_rank == 0){
		printf("Main Vector: ");
		print_intVec(bigArray, SIZE);
		printf("\nNormalized: ");
		print_doubleVec(bigResult, SIZE);
		printf("\nGlobal Sum of Squares: %d\nSquare Root: %f\n",globalSumOfSquares, globalSqrt);
	}

	if(VERBOSE){
		printf("\nProcess %d ------>\nSlice: ",my_rank);
		print_intVec(slice, sliceSize);
		printf("\nSliceNorm: ");
		print_doubleVec(sliceNorm, sliceSize);
		printf("\nLocal Sum: %d ",sliceSum);
		printf("\nReceived Global Square Root: %f\n",globalSqrt);
	}
	
	MPI_Finalize();
	//please no MPI stuff after this line, okay :)
	return 0;
}
