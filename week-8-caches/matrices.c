#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <stdbool.h>

#define CHUNK 100

//=========================== Serial functions =================================

bool PRINT_MATRICES = false;
bool PRINT_TIMING = true;

void initializeMatrix(double *matrix, int rows, int cols){
  unsigned short xi[3] = {1,9,99};
  int i, j;
  for(i = 0; i < rows; i++){
    for(j = 0; j< cols; j++){
      matrix[(i*rows)+j] = erand48(xi);
    }
  }
}

void printMatrix(double *matrix, int rows, int cols, char* label){
  int i, j;
  printf("\n%s\n", label);
  for(i = 0; i < rows; i++){
    for(j = 0; j< cols; j++){
      printf("%f ",matrix[(i*rows)+j]);
    }
    printf("\n");
  }
}

void printTimeData(double start, double end, char* label){
  double result = end - start;
  printf("%s: %f\n", label, result);
}


void addMatrix(double *mat1, double *mat2, double *result, int rows, int cols){
  int i, j;
  for(i = 0; i<rows; i++){
    for(j= 0; j<cols; j++){
      int here = (i*rows)+j;
      result[here] = mat1[here] + mat2[here];
    }
  }
}

void transposeMatrix(double *mat, double *result, int rows, int cols){
  int i, j;
  for(i = 0; i<rows; i++){
    for(j= 0; j<cols; j++){
      int here = (i*rows)+j;
      int there = (j*rows)+i;
      result[there] = mat[here];
    }
  }
}

//========================== Parallel functions ================================

void initializeMatrix_p(double *matrix, int rows, int cols, int nthreads){
  unsigned short xi[3] = {1,9,99};
  #pragma omp parallel for num_threads(nthreads)
  for(int i = 0; i < rows; i++){
    for(int j = 0; j< cols; j++){
      matrix[(i*rows)+j] = erand48(xi);
    }
  }

}

void addMatrix_p(double *mat1, double *mat2, double *result, int rows, int cols, int nthreads){
  #pragma omp parallel for schedule(dynamic, CHUNK) num_threads(nthreads)
  for(int i = 0; i<rows; i++){
    for(int j = 0; j<cols; j++){
      int here = (i*rows)+j;
      result[here] = mat1[here] + mat2[here];
    }
  }
}

void transposeMatrix_p(double *mat, double *result, int rows, int cols, int nthreads){
  #pragma omp parallel for schedule(dynamic, CHUNK) num_threads(nthreads)
  for(int i = 0; i<rows; i++){
    for(int j= 0; j<cols; j++){
      int here = (i*rows)+j;
      int there = (j*rows)+i;
      result[there] = mat[here];
    }
  }
}


int main(int argc, char *argv[])
{

  int ROWS = 0;
  int COLS = 0;
  int numthreads = 1;
  // THIS WILL BE YOUR RANDOM SEED
  // YES IT IS NON-RANDOM - Read the lab!
  unsigned short xi[3] = {1,9,99};
  double somenum = erand48(xi);

  if (argc > 4){
    printf("usage: matrices <rows> <cols>\n");
    printf("usage: matrices <rows> <cols> <numthreads>\n");
    exit(1);
  }
  else{
      ROWS = atoi(argv[1]);
      COLS = atoi(argv[2]);
      if(argc > 3){
        numthreads = atoi(argv[3]);
      }
      printf("\nRows: %d, Cols: %d, Threads: %d\n",ROWS,COLS,numthreads);
  }

  double start = omp_get_wtime();

  double * matrix1 =  calloc(ROWS*COLS,sizeof(double));
  double * matrix2 =  calloc(ROWS*COLS,sizeof(double));
  double * matrix3 =  calloc(ROWS*COLS,sizeof(double));

  double time_init_serial1 = omp_get_wtime();
  initializeMatrix(matrix1, ROWS, COLS);
  double time_init_serial2 = omp_get_wtime();
  initializeMatrix(matrix2, ROWS, COLS);
  double time_init_serial3 = omp_get_wtime();

  if(PRINT_MATRICES){
    printf("\n<--- Serial Section --->\n");
    printMatrix(matrix1,ROWS,COLS, "matrix1: serial init");
    printMatrix(matrix2,ROWS,COLS, "matrix2: serial init");
  }

  double time_add_serial = omp_get_wtime();
  addMatrix(matrix1,matrix2,matrix3,ROWS,COLS);
  double time_add_serial_end = omp_get_wtime();

  if(PRINT_MATRICES){
    printMatrix(matrix3,ROWS,COLS, "matrix3: matrix1 + matrix2");
  }

  double time_transpose_serial = omp_get_wtime();
  transposeMatrix(matrix1, matrix3, ROWS, COLS);
  double time_transpose_serial_end = omp_get_wtime();

  if(PRINT_MATRICES){
    printMatrix(matrix3,ROWS,COLS, "matrix3: transposition of matrix1");
  }

  double end_serial = omp_get_wtime();

  matrix1 =  calloc(ROWS*COLS,sizeof(double));
  matrix2 =  calloc(ROWS*COLS,sizeof(double));
  matrix3 =  calloc(ROWS*COLS,sizeof(double));

  double time_init_parallel1 = omp_get_wtime();
  initializeMatrix_p(matrix1, ROWS, COLS, numthreads);
  double time_init_parallel2 = omp_get_wtime();
  initializeMatrix_p(matrix2, ROWS, COLS, numthreads);
  double time_init_parallel3 = omp_get_wtime();

  if(PRINT_MATRICES){
    printf("\n<--- Parallel Section --->\n");
    printMatrix(matrix1,ROWS,COLS, "matrix1: parallel init");
    printMatrix(matrix2,ROWS,COLS, "matrix2: parallel init");
  }

  double time_add_parallel = omp_get_wtime();
  addMatrix_p(matrix1,matrix2,matrix3,ROWS,COLS, numthreads);
  double time_add_parallel_end = omp_get_wtime();

  if(PRINT_MATRICES){
    printMatrix(matrix3,ROWS,COLS, "matrix3: matrix1 + matrix2");
  }

  double time_transpose_parallel = omp_get_wtime();
  transposeMatrix_p(matrix1, matrix3, ROWS, COLS, numthreads);
  double time_transpose_parallel_end = omp_get_wtime();

  if(PRINT_MATRICES){
    printMatrix(matrix3,ROWS,COLS, "matrix3: transposition of matrix1");
  }

  double end_parallel = omp_get_wtime();

  if(PRINT_TIMING){
    printf("\n<--- Time Data --->\n");

    printTimeData(time_init_serial1, time_init_serial2, "Serial Init 1");
    printTimeData(time_init_serial2, time_init_serial3, "Serial Init 2");
    printTimeData(time_add_serial, time_add_serial_end, "Serial Add");
    printTimeData(time_transpose_serial, time_transpose_serial_end, "Serial Transpose");

    printTimeData(time_init_parallel1, time_init_parallel2, "Parallel Init 1");
    printTimeData(time_init_parallel2, time_init_parallel3, "Parallel Init 2");
    printTimeData(time_add_parallel, time_add_parallel_end, "Parallel Add");
    printTimeData(time_transpose_parallel, time_transpose_parallel_end, "Parallel Transpose");
}
 //free(MAT); //don't FREE! C99 takes care of it if allocated like this
 //(instead of via malloc)
}
