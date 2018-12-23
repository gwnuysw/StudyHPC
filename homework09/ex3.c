#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#define N 120

void main(int argc, char* argv[])
{
   int pid, np, i, tag = 0;
   float *A, sum;

   MPI_Comm_Status status;

   MPI_Comm_size(MPI_COMM_WORLD, &np);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);
   local_N = N/np;
   if(pid == 0){
      A = (float*)malloc(sizeof(float)*N);
      for(i = 0; i < N;i++)
         A[i] = (float)i;
   }
   local_A = (float *)malloc(sizeof(float)*local_N);

   /*for(i = 0; i < N; i++)
      sum += A[i];*/
   MPI_Scatter(A, N/np, MPI_FLOAT, local_A, local_N, MPI_FLOAT, 0, MPI_COMM_WORLD);
   
   local_sum = 0.0;
   for(i = 0; i < local_N;i++)
      local_sum += local_A[i];

   MPI_Reduce(&local_sum, &sum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD)

   if(pid == 0) printf("%f\n", sum);
   if(pid == 0) free(A);
   printf("%f\n",sum);

   free(A);
   MPI_Finalize();
}
