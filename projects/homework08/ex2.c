#include <stdio.h>
#include <math.h>
#include "mpi.h"
main(int argc, char* argv[]){
   int pid, np, i, tag = 0;
   int prod2 = 1;
   int prod;
   int bits, partner;
   int N;
   MPI_Status status;
   MPI_Init(&argc, &argv);

   MPI_Comm_size(MPI_COMM_WORLD, &np);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);

   prod = pid+1;
   N = (int)(log(np)/log(2));
   bits = np;
   for(i = 0; i < N; i++){
      if(pid < bits){
         bits = bits >> 1;
         partner = pid^bits;
         if(pid < bits){
            MPI_Recv(&prod2, 1, MPI_INT, partner, tag,MPI_COMM_WORLD, &status);
            prod *= prod2;
         }
         else{
            MPI_Send(&prod, 1, MPI_INT, partner, tag, MPI_COMM_WORLD);
         }
      }
   }
   if(pid == 0){
      printf("prod2 : %d\n", prod);
   }
   
   MPI_Finalize();
}
