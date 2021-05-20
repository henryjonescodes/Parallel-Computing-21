#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

void initializeMatrix(double *matrix, int rows, int cols){
  unsigned short xi[3] = {1,9,99};
  int i, j;
  for(i = 0; i < rows; i++){
    for(j = 0; j< cols; j++){
      matrix[(i*rows)+j] = erand48(xi);
    }
  }
}

void printMatrix(double *matrix, int rows, int cols){
  int i, j;
  for(i = 0; i < rows; i++){
    for(j = 0; j< cols; j++){
      printf("%f ",matrix[(i*rows)+j]);
    }
    printf("\n");
  }
  printf("\n");
}

int main(int argc, char *argv[])
{

  int ROWS = 0;
  int COLS = 0;
  // THIS WILL BE YOUR RANDOM SEED
  // YES IT IS NON-RANDOM - Read the lab!
  unsigned short xi[3] = {1,9,99};
  double somenum = erand48(xi);

  if (argc != 3){
    printf("usage: matrices <rows> <cols>\n ");
    exit(1);
  }
  else{
      ROWS = atoi(argv[1]);
      COLS = atoi(argv[2]);
      printf("Rows: %d, Cols: %d\n",ROWS,COLS);
  }

  //double MAT[][] = calloc(ROWS*COLS,sizeof(double));

//double MAT[ROWS][COLS];
double * MAT =  calloc(ROWS*COLS,sizeof(double));

initializeMatrix(MAT, ROWS, COLS);
printMatrix(MAT,ROWS,COLS);

 //free(MAT); //don't FREE! C99 takes care of it if allocated like this
 //(instead of via malloc)
}
