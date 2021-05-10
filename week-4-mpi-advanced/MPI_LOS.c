#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h> //for random seed
#include <math.h> //need to compile with -m

//to compile: mpicc -std=c99 -Wall MPI_LOS.c -o MPI_LOS
//to run (simple): mpirun ./MPI_LOS
//to run (multi-process): mpirun -np 4 ./MPI_LOS


#define INITIAL_HEIGHT 100
#define DX 100

/*
MakeTerrain makes a local terrain by accumulating +/- 2 increments
*/
void MakeTerrain(int *localworld, int localsize,int my_rank)
{
		//starting height
	int upper =2;
	int lower = -2;
	if (my_rank == 0)
		localworld[0] = INITIAL_HEIGHT;
	else
		localworld[0] = (rand() % (upper - lower + 1)) + lower;
    int i;
	for (i = 1; i < localsize; i++)
	{
		int delta = (rand() % (upper - lower + 1)) + lower;
		localworld[i] = localworld[i-1]  + delta;
	}
	//for (i = 0; i < localsize; i++)
	//	printf("%d %d\n", i, localworld[i]);
}


void PrintTerrain(int *localworld, int localsize,int my_rank)
{
	int i;
	int offset = DX*my_rank*localsize;
	for (i = 0; i < localsize; i++)
			printf("(%d,%d),", (DX*i)+offset, localworld[i]);
    printf("\n");

}
/*
Calculate Angles:
in: localworld, localsize, my_rank
out: outangles
out: maxangles: the prefix max scan of local angles

calculates arctan between starting location (0,INITIALHEIGHT)
and current point.  Assume x location increases by 100 for every x index

*/
void CalculateAngles(double *outangles, double *maxangles, int *localworld, int localsize,int my_rank)
{

	int mystartx = my_rank*localsize;
	//we're never going to have a 180degree angle, so set this as max
	double maxanglesofar = -3.14;
	int i;
	for (i = 0; i < localsize ; i++)
	{
		int x = (mystartx+i)*DX;
		int y = localworld[i] - INITIAL_HEIGHT;
		double angle = atan2(y,x);

		outangles[i] = angle;
		if (i == 0)
			maxanglesofar = -3.14;
		else
			maxanglesofar = angle >= maxanglesofar ? angle: maxanglesofar;
		maxangles[i] = maxanglesofar;
		printf("(%d,%d,%f,%f),", x, y,angle,maxanglesofar);
	}
	printf("\n");

}


/*

FixAnglesAndFindVisible:

given a local array of viewing angles
and a local array of max-so-far viewing angles (cumulative max)
AND given the cumulative max of the prior tasks
determine which points are viewable.

in: angles (local array of viewing angles)
in: maxangles (local array of cumulative max viewing angles)
out: visible (char array of whether point is visible.)
*/
void FixAnglesAndFindVisible(double *angles, double *maxangles, char *visible, double scanmax, int mysize)

{
	int i;
	for (i = 0; i < mysize; i++)
	{
		double actualmax = maxangles[i] >= scanmax ? maxangles[i] : scanmax;
		visible[i] = angles[i]>= actualmax ? 1 : 0;
		printf("scanmax: %f, maxi %f, actualmax: %f, %d\n",scanmax,angles[i],actualmax,visible[i]);
	}
	printf("\n");

}

int main(int argc, char *argv[]){


	int        num_procs;               /* Number of processes    */
	int        my_rank;               /* My process rank        */
	//Don't put any MPI stuff before this line
	MPI_Init(&argc, &argv);  //sets up the MPI. Always this line!
	/* Get the number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	/* Get my rank among all the processes */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	// do some stuff

	srand(time(NULL) + my_rank);

	// Start the clock
	double start_time = MPI_Wtime();

	int WORLDSIZE = 20;
	int mysize = WORLDSIZE/num_procs;

	int * localworld = malloc(mysize*sizeof(int));

	int i;
	int upper = 2;
	int lower = -2;
	int initialHeight = 100;



	MakeTerrain(localworld,mysize,my_rank);

	//int sum = localworld[0];

	//exclusive prefix scan gives every node prefix operator applied to all PRIOR
	//nodes, but not to itself.
	// eg prefix scan [1,2,3,4] -> [2,4,7,11]
	// vs exscan      [1,2,3,4] -> [1,3,6,10]
	// PART 1: USE Exscan to accumulate the running total
	// of the last item in every localterain prior to me
	// and add it as an offset to everything in my terrain
	int offset = 0;
	//call exscan here
	MPI_Exscan(&localworld[mysize-1], &offset, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);


	//debug print to verify excan worked
	printf("%d offset %d\n", my_rank, offset);
	// PART 1:
	// next correct altitudes based upon received offset
	int z;
	for(z = 0; z < mysize; z++){
		localworld[z] = localworld[z] + offset;
	}

	//debug print to verify altitudes are corrected
	PrintTerrain(localworld,mysize,my_rank);
   //now to calculate arctans
   //every angle from starting position to current value
   double * angles = malloc(mysize*sizeof(double));
   //the max angle so far
   double * maxangles = malloc(mysize*sizeof(double));

   CalculateAngles(angles,maxangles,localworld,mysize,my_rank);

	//now we need to find the max angle found by every node
	//this time by prefix max scanning the last angle in every array

	//STEP 2: use exscan again to find the *max* value of every node'
	// maxangles array (the last value)
	double scanmax = 0;

	//call excan here
	MPI_Exscan(&maxangles[mysize-1], &scanmax, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

	//debugginb print
	printf("%d max is %f, scanmax is %f \n",my_rank,maxangles[mysize-1],scanmax);

	char * visible = malloc(mysize*sizeof(char));

	if (my_rank == 0)
	{
		//a juke to make sure that the point at index 1 is always visible
		scanmax = -3.14;
	}
	FixAnglesAndFindVisible(angles,maxangles,visible,scanmax,mysize);

	char * globalvis = malloc(WORLDSIZE*sizeof(char));

    //Step 3: gather the world and the visibility array here

	MPI_Gather(visible, mysize, MPI_CHAR, globalvis, mysize, MPI_CHAR, 0, MPI_COMM_WORLD);

	if (my_rank == 0)
	{
		for (i = 0; i < WORLDSIZE; i++)
			printf("%d",globalvis[i]);
	    printf("\n");
	}

	// End the clock
	double end_time = MPI_Wtime();

	if (my_rank == 0)
	{
		printf("That Took (seconds): %f", end_time-start_time);
	}

	MPI_Finalize();
	//please no MPI stuff after this line
	return 0;
}
