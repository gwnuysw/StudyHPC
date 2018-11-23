#include "mpi.h"
#include <stdio.h>

main(int argc, char* argv[])
{
   int np, pid;
   int source, dest;
   int tag = 0;
   MPI_Status status;
   MPI_Init(&argc, &argv);
   
   MPI_Comm_size(MPI_COMM_WORLD, &np);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);

   printf("number of processes : %d, process id: %d: \n", np, pid);

   MPI_Finalize();
}
