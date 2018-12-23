# include "mpi.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

main(int argc, char* argv[])
{
   int np, pid;
   int source,  dest;
   int tag = 0;
   MPI_Status status;

   char* beforeDot;
   char message[100];
   char hostname[7];

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &np);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);

   if(pid != 0)
   {
      //sprintf(message, "Greetings from processr %d", pid);
      gethostname(hostname, 6);
      dest = 0;
      MPI_Send(hostname, 7, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
   }
   else {
      for(source = 1; source <np; source++){
         MPI_Recv(hostname, 7, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
         beforeDot = strtok(hostname,".");
         printf("%d %s\n", source, beforeDot);   
      }
   }
   MPI_Finalize();
}
