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

   char hostname[7];

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &np);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);

   if(pid != 0)
   {
       
      gethostname(hostname, 6);
      beforeDot = strtok(hostname,".");
      printf("pid != 0 send athors %s\n",beforeDot);
      dest = 0;
      MPI_Send(beforeDot, strlen(beforeDot)+1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
   }
   else {
      for(source = 1; source <np; source++){
         MPI_Recv(beforeDot, 7, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
         printf("%d %s\n", pid, beforeDot);   
      }
   }
   MPI_Finalize();
}
