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

   int a[24];
   int b[24];
   
   int c[24];
   int i;
   int k;
   
   int *local_a, *local_b, *local_c;

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &np);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);

   if(pid == 0){
      for(i = 0; i < 24; i++){
         a[i] = i + 10;
         b[i] = i + 20;
      }
   }
   else{
      local_a = (int *)malloc(sizeof(int)*(24/np));
      local_b = (int *)malloc(sizeof(int)*(24/np));
      local_c = (int *)malloc(sizeof(int)*(24/np));
   }
   MPI_Scatter(a, 24/np, MPI_INT, local_a, 24/np, 0, MPI_COMM_WORLD);
   MPI_Scatter(b, 24/np, MPI_INT, local_b, 24/np, 0, MPI_COMM_WORLD);
   if(pid != 0){
      for(i = 0; i <24/np; i++){
         local_c[i] = local_a[i] + local_b[i];
      }
   }
   MPI_Gather(local_c, 24/np, MPI_INT, 
   /*if(pid != 0)
   {
      //sprintf(message, "Greetings from processr %d", pid);
      source = 0;
      MPI_Recv(a+pid*(24/np), 24/np, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
      MPI_Recv(b+pid*(24/np), 24/np, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
      for(i = pid*(24/np); i <= pid*(24/np)+6; i++){
         c[i] = a[i] + b[i];
      }
      dest = 0;
      MPI_Send(c+pid*(24/np), 24/np, MPI_INT, dest, tag, MPI_COMM_WORLD);
   }
   else {
      for(i = 0; i < 24; i++){
         a[i] = i + 10;
         b[i] = i + 20;
      }
      for(dest = 1; dest < np; dest++){
         MPI_Send(a+dest*(24/np), 24/np, MPI_INT, dest, tag, MPI_COMM_WORLD);
         MPI_Send(b+dest*(24/np), 24/np, MPI_INT, dest, tag, MPI_COMM_WORLD);
      }
      for(source = 1; source < np; source++){
         MPI_Recv(c+source*(24/np), 24/np, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
      }
      for(i = 0; i < 6; i++){
         c[i] = a[i] + b[i];
      }
      for(i = 0; i < 24; i++){
         printf("%d ",c[i]);
      }
   }*/
   MPI_Scatter(
   
   MPI_Finalize();
}
