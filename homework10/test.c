# include <stdio.h>
# include <math.h>
# include <sys/time.h>
# include <stdlib.h>
# include "mpi.h"

int millisecond()
{
   struct timeval tv;
   gettimeofday(&tv, NULL);
   return tv.tv_sec * 1000 + tv.tv_usec/1000;
};
int main(int argc, char *argv[])
{
   long N,i;
   int np, pid, tag = 0;
   int start, stop, excution;
   double *A, *B, *localA, *localB, localC, dotprod, MFLOPS, localN;
   MPI_Status status;

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &np);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);

   N = 200000000;

   localN = N/np;

   if(pid == 0){
      A = (double*)malloc(sizeof(double)*N);
      for(i = 0; i < N; i++){
         A[i] = (double)i;
      }
      printf("inicialized\n");
   }

   localA = (double*)malloc(sizeof(double)*localN);
   localC = 0;

   MPI_Scatter(A, localN, MPI_DOUBLE, localA, localN, MPI_DOUBLE, 0, MPI_COMM_WORLD);

   for(i = 0; i < localN; i++){
      localC += localA[i];
   }

   MPI_Reduce(&localC, &dotprod, localN, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
   printf("reduced\n");

   if(pid == 0){
      stop = millisecond();
      MFLOPS = 2.0 * N / (1000.0 * (double)excution);
      printf("dotprod : %5.10lf, MFLOPS : %5.10lf\n", dotprod, MFLOPS);
      printf("excution time : %d milliseconds\n", excution);
      free(A);
      free(B);
   }

   free(localA);
   free(localB);
   MPI_Finalize();
}
