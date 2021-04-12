#define _XOPEN_SOURCE

#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

bool CONSTANT_VECTORS = false;
bool VERBOSE_TESTS = false;

//to compile: mpicc -std=c99 -Wall MPI_vector_dot.c -o MPI_vector_dot
//to run (simple): mpirun ./MPI_vector_dot
//to run (multi-process): mpirun -np 4 ./MPI_vector_dot

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
double * fill_vector(double *vec, int size, double filler[])
{
  int index;
  for (index= 0; index< size; index++ ) {
   vec[index] = filler[index];
  }
  return vec;
}

void print_vector_p(double *vec, int size)
{
	// printf("printing");
  double *p;
  double * vec_end = vec+size;
  for (p = vec; p < vec_end; p++)
  {
    printf("%f,", *p);
  }
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
    printf("usage: MPI_vector_dot <size>\n ");
    exit(1);
  }
  else {
      SIZE = atoi(argv[1]);
  }

	//Start the clock
	double start_time = MPI_Wtime();

	//Storage for totals and partial totals
	double localsum = 0;
	double globalsum = 0;

	//Allocate space here so we can use the same for loop for all additions
	double * vec1 = calloc(SIZE,sizeof(double));
	double * vec2 = calloc(SIZE,sizeof(double));

	//Non-root processes ecieve vectors from root process
	if (my_rank != 0) {
		MPI_Recv(vec1,SIZE,MPI_DOUBLE,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		MPI_Recv(vec2,SIZE,MPI_DOUBLE,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	}
	else
	{
		//Toggle between hard-coded and randomized vectors
		if(!CONSTANT_VECTORS){
			//Randomize using pointer method
			vec1 = randomize_vector_p(vec1, SIZE);
			vec2 = randomize_vector_p(vec2, SIZE);
		} else {
			//Hard-coded test vectors (expected product vec1 • vec2 = 2.776287700675)
			double vec1_array[] = {0.315817,0.021303,0.933138,0.463567,0.606694,
							0.708238,0.638140,0.563823,0.733255,0.401841};
			double vec2_array[] = {0.291766,0.439373,0.233446,0.231762,0.968261,
							0.122894,0.741578,0.943867,0.410790,0.916815};
			vec1 = fill_vector(vec1, SIZE, vec1_array);
			vec2 = fill_vector(vec2, SIZE, vec2_array);
		}

		//Send vectors to non-root processes
		int dest;
		for(dest = 1; dest < comm_sz; dest++){
			MPI_Send(vec1,SIZE,MPI_DOUBLE,dest,0,MPI_COMM_WORLD);
			MPI_Send(vec2,SIZE,MPI_DOUBLE,dest,0,MPI_COMM_WORLD);
		}
	}

	//Do the dot product math, same for all processes
	int i;
	for (i = my_rank; i < SIZE; i+=comm_sz)
	{
		localsum += vec1[i] * vec2[i];
	}

	//Non-root processes send back to root
	if(my_rank != 0){
		MPI_Send(&localsum,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
	}
	//Root process cleans things up
	else {
		//add root process's work
		globalsum = globalsum + localsum;

		//recieve partial totals
		int src;
		for(src = 1; src < comm_sz; src++){
			double tempsum = 0;
			MPI_Recv(&tempsum,1,MPI_DOUBLE,src,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			globalsum = globalsum + tempsum;
		}

		//Stop the clock
		double end_time = MPI_Wtime();

		if(VERBOSE_TESTS){
			//Print out the vectors being added
			printf("size: %d\n",SIZE);
			printf("Vector 1:");
			print_vector_p(vec1,SIZE);
			printf("\nVector 2:");
			print_vector_p(vec2,SIZE);
		}

		//print dot product and timing
		printf("\nvector1 • vector2 = %f",globalsum);
		printf("\n\nFinished on %d processes in %fms",
					comm_sz, (end_time-start_time));
	}

	MPI_Finalize();
	//please no MPI stuff after this line, okay :)
	return 0;
}
