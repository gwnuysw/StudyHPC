# include "mpi.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

main(int argc, char* argv[])
{
   int np, pid;
   int source,  dest;
   int tag = 0;
   MPI_Group group_world, group_even;
   MPI_Comm comm_even;
   int *newrank, i;
   int data;
   MPI_Status status;

   char* beforeDot;
   char message[100];
   char hostname[7];

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &np);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);

   newrank = (int*)(malloc((np/2)*sizeof(int)));
   for(i = 0; i < np/2; i++){
      newrank[i] = i*2;
   }
   MPI_Comm_group(MPI_COMM_WORLD, &group_world);
   MPI_Group_incl(group_world, np/2, newrank, &group_even);
   MPI_Comm_create(MPI_COMM_WORLD, group_even, &comm_even);

   data = pid;

   if(!(pid%2)) MPI_Bcast(&data, 1, MPI_INT, 2, comm_even);
   printf("%d %d\n", pid, data);

   MPI_Finalize();
}
