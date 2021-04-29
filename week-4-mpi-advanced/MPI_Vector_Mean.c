#define _XOPEN_SOURCE

#include <mpi.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>





//to compile: mpicc -std=c99 -Wall MPI_Vector_Mean.c -o MPI_Vector_Mean
//to run (simple): mpirun ./MPI_Vector_Mean
//to run (multi-process): mpirun -np 4 ./MPI_Vector_Mean

bool CONSTANT_VECTOR = false;
bool VERBOSE = false;
int MAX_VALUE = 10;
int MIN_VALUE = 0;

void print_intVec(int *vec, int size,int myId)
{
	printf("(%d): ",myId);
  int *p;
  int * vec_end = vec+size;
  for (p = vec; p < vec_end; p++)
  {
    printf("%d ", *p);
  }
	printf("\n");
}

void print_doubleVec(double *vec, int size)
{
  double *p;
  double * vec_end = vec+size;
  for (p = vec; p < vec_end; p++)
  {
    printf("%f ", *p);
  }
	printf("\n");
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

int main(int argc, char *argv[]){


	int        comm_sz;               /* Number of processes    */
	int        my_rank;               /* My process rank        */
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
			printf("usage: MPI_Vector_Mean <size>\n ");
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
	int partialSum = 0;
	int globalSum;
	double result;

	//All processes make their own random arrays (of ints)
	slice = randomize_intVec(slice, sliceSize);

	int i;
	for(i = 0; i < sliceSize; i ++){
		partialSum += slice[i];
	}
	//int MPI_Reduce(const void *sendbuf, void *recvbuf, int count,
	//							 MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm)

	MPI_Reduce(&partialSum, &globalSum, 1, MPI_INT, MPI_SUM,0, MPI_COMM_WORLD);

	//Two if statements to exclude print from timing
	if (my_rank == 0) {
		result = (double)globalSum/(double)SIZE;
	}

	// Start the clock
	double end_time = MPI_Wtime();
	double elapsed = end_time - start_time;

	if (my_rank == 0) {
		printf("(%d): Mean = %f\n",my_rank, result);
		printf("(%d): Time(sec) = %f\n",my_rank, elapsed);
	}

	if(VERBOSE){
		print_intVec(slice, sliceSize,my_rank);
		printf("(%d): Local Sum = %d\n",my_rank, partialSum);
	}

	MPI_Finalize();
	//please no MPI stuff after this line
	return 0;
}
