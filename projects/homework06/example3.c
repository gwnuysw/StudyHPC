# include "mpi.h"
#include <stdio.h>
#include <string.h>

main(int argc, char* argv[])
{
   int np, pid;
   int source, dest;
   int tag = 0;
   MPI_Status status;

   char message[100];

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &np);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);

   if(pid != 0)
   {
      sprintf(message, "Greetings from processor %d",pid);
      dest = 0;
      MPI_Send(message, strlen(message)+1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
   }
   else{
      for(source = 1; source < np; source++){
         MPI_Recv(message, 100, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
         printf("%d: %s\n",pid, message);
      }
   }
   MPI_Finalize();
}
