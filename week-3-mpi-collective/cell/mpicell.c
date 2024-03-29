#include <stdio.h>
#include <time.h>
#include <string.h>
#include "CellAut.h"
#include "mpi.h"

/* mpicell.c
 author: John Rieffel
*/


// We need to get values from our neighbors, and send values to our neighbors
// how to do it without deadlock?
// MPI_Send is non-blocking, except when it isn't
//
// When this function returns,
// leftval should contain the value sent by your left neighbor
// and rightval should contain the value sent by your right neighbor

void DistributeLeftAndRightVals(int myID, int numprocs, int *mylocalcells, int localsize, int *leftval, int *rightval)
{
   int rightID=(myID+1)%numprocs;  //neighbor to your right
   int leftID=(myID-1+numprocs)%numprocs; //neighbor to your left

   int myfirst = mylocalcells[0];
   int mylast = mylocalcells[localsize-3]; //note that we have to extra cells of padding.

   MPI_Status status;

   if(myID-1 < 0){
     leftID = numprocs - 1;
   }
   if(myID + 1 > numprocs-1){
     rightID = 0;
   }

   // printf("I am node %d, recieving left from node %d\nrecieving right from node %d\n", myID, leftID,rightID);

   //CSC-333 inclass: put communication in here!
	// STEP TWO
  //replace these lines below with your correct communications

  if(myID % 2 == 0){
    //Send and recieve to the right
    MPI_Send(&mylast,1,MPI_INT,rightID,0,MPI_COMM_WORLD);
    MPI_Recv(rightval,1,MPI_INT,rightID,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

    //Send and recieve to the left
    MPI_Send(&myfirst,1,MPI_INT,leftID,0,MPI_COMM_WORLD);
    MPI_Recv(leftval,1,MPI_INT,leftID,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
  } else {
    //Revieve and send to the left
    MPI_Recv(leftval,1,MPI_INT,leftID,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    MPI_Send(&myfirst,1,MPI_INT,leftID,0,MPI_COMM_WORLD);

    //Revieve and send to the right
    MPI_Recv(rightval,1,MPI_INT,rightID,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    MPI_Send(&mylast,1,MPI_INT,rightID,0,MPI_COMM_WORLD);
  }


   // *leftval = 0;
   // *rightval = 0;
   // printf("I am node %d, I got %d from left node %d and %d from right node %d\n", myID,*leftval,leftID,*rightval,rightID);

}

void MPIRunCellWorld(int myID, int numprocs, int *localcells, int sizeOfMyWorld, int iterations, int rule)
{
   int leftval = 0;
   int rightval = 0;
   int *newcells = MakeCellWorld(sizeOfMyWorld);

  int itercount = 0;
  for (itercount = 0; itercount < iterations; itercount++)
     {
             // printf("node %d before rules\n",myID);
             // printWorld(localcells,localsize,myID);
      // after calling this
      // the variables leftval and rightval
      // should have the values from the neighboring worlds.

       // double distribute_start_time = MPI_Wtime();

       DistributeLeftAndRightVals(myID,numprocs, localcells, sizeOfMyWorld, &leftval, &rightval);

       // double distribute_end_time = MPI_Wtime();
       // double elapsed = distribute_end_time = distribute_start_time;
       //
       // printf("\n(%d:%d): Distribution took (seconds): %f",myID, numprocs, elapsed);


       // shouldn't have to touch this, it should just work
       // Assuming we've padded localworld with two extra indexes
       //We can put the right neigbhor's value at the second to last index
       // and the left neighbor's value at the last index
       //
       // i.e. if size of localworld L is actually five (5)
       // index: 0     1    2    3    4    5  6
       //       L[0] L[1] L[2] L[3] L[4]   R  L
       // and then if we treat the array as a torus/ring, the math works
       // with the same function as the serial version.
       localcells[sizeOfMyWorld-2] = rightval;
       localcells[sizeOfMyWorld-1] = leftval;
       int locali = 0;
       for (locali = 0; locali < sizeOfMyWorld; locali++) {
	        ApplyRuleAtLoc(localcells,newcells,locali,sizeOfMyWorld,rule);
          // printWorld(localcells,sizeOfMyWorld,myID);
        }
       // the world becomes the new world
       // and new world becomes the old world
       // (this way we only have to allocate the array once)
       memcpy((void *)localcells,(void *)newcells,sizeOfMyWorld*sizeof(int));

     }
     free(newcells);

}
int main(int argc, char *argv[])
{
  //command line arguments are passed in as an array of strings!
  //where argc says how many arguments there are
  //(note the executable itself is the first argument, so argc is always 1)
   int i;
   int id;               /* Process rank */
   int p;                /* Number of processes */

   MPI_Init (&argc, &argv);
   MPI_Comm_rank (MPI_COMM_WORLD, &id);
   MPI_Comm_size (MPI_COMM_WORLD, &p);

   int PRINT_FLAG = 0;
   int WORLDSIZE = 128;
   int iterstep = 1;
   int curiters = 0;
   int maxiters = 100;
   int rule = 30;

    if (argc > 1 && argc <= 5) {
      rule = atoi(argv[1]);
      if (argc > 2 )
      {
          WORLDSIZE = atoi(argv[2]);
          if (argc > 3)
          {
              maxiters = atoi(argv[3]);
              if (argc > 4)
              {
                int flag1 = atoi(argv[4]);
                if(flag1 != 1 && flag1 != 0){
                  printf("print flag must be binary");
                } else {
                  PRINT_FLAG = flag1;
                }
              }
          }
      }
    }
    else{
      printf("usage: runcell <rulenumber>\n");
      printf("usage: runcell <rulenumber> <worldsize>\n");
      printf("usage: runcell <rulenumber> <worldsize> <maxiters>\n");
      printf("usage: runcell <rulenumber> <worldsize> <maxiters> <printflag>\n");
      MPI_Finalize();
      exit(1);
    }

  // double start_time = MPI_Wtime();

  int *localcells; //local array
  int localsize = (WORLDSIZE/p); //how many items you care about
  int paddedsize = localsize + 2;  //a
                                   //plus two extra indexes, one for each neighbor

   if (WORLDSIZE%p != 0)
     {
       printf("Worldsize must be divisible by processors!");
       MPI_Finalize();
       exit(1);
     }

   if ((localcells = calloc(paddedsize,sizeof(int))) == NULL) {
	     printf("Memory allocation error!");
	     MPI_Finalize();
	     exit(1);
   }

   //everyone creates their own pointer to the world
   // so that collective communication calls work
   int *mycellworld;

   // but only the root node actually needs to allocate
   if (id == 0)
     {
       printf("(%d:%d): worldsize: %d, localsize: %d maxiters: %d \n",id,p,WORLDSIZE,localsize,maxiters);
       mycellworld = MakeCellWorld(WORLDSIZE);
       InitCellWorld(mycellworld,WORLDSIZE);
       if(PRINT_FLAG == 1){
         printWorld(mycellworld,WORLDSIZE,id);
       }
     }



   //scatter global copy to rest of nodes' local world
   // CSC-333 inclass: put code here
	// STEP ONE

  MPI_Scatter(mycellworld,localsize,MPI_INT,localcells,localsize,MPI_INT,0,MPI_COMM_WORLD);


  // NOTE: you want to scatter
  // LOCALSIZE-sized chunks of the world from the root node
  // to each node
  // (yes it is okay that you are scattering N items
  // into an array of size N+2)

   //now everyone has their own slice of the world!
   //
   // printf("I am node %d, my local world is now:\n",id);
   // printWorld(localcells,localsize,id);

   for (curiters = 0; curiters < maxiters; curiters++)
     {
       //every node runs MPIRunCellWorld over its local world
       // notice that the local world is of size paddedsize
       // to accomodate the values recieved from Left and Right neigbhors
       //and make the running of the cell rules simpler.
       MPIRunCellWorld(id,p,localcells,paddedsize,iterstep,rule);
       //     printf("I am node %d, my local world is now:\n",id);
       //printWorld(localcells,localsize,id);



       //CSC333 inclass
       //now we need to use Gather to get the distributed data back to node 0
       //so we can print it out
       //hint: works very similarly to scatter
       // Warning your code won't get much speedup if you call this every iteration!
		 // STEP THREE

      if (PRINT_FLAG == 1){
          MPI_Gather(localcells, localsize, MPI_INT, mycellworld,localsize,MPI_INT, 0, MPI_COMM_WORLD);
      if (id == 0)
    	  {
    	    printWorld(mycellworld,WORLDSIZE,id);
    	  }
     }
  }
	if (PRINT_FLAG == 0){
    MPI_Gather(localcells, localsize, MPI_INT, mycellworld,localsize,MPI_INT, 0, MPI_COMM_WORLD);
    // double end_time = MPI_Wtime();
    // double elapsed = end_time - start_time;

    if (id == 0)
  	  {
  	    printWorld(mycellworld,WORLDSIZE,id);
        // printf("(%d:%d): That took (seconds): %f\n",id,p,elapsed);
  	  }
  }

  MPI_Finalize();
}
