# include <stdio.h>
# include <string.h>
# include "mpi.h"
 
int main(int argc, char *argv[])
{
   char msg[10];
   int n;
   float f;

   int pid, position, tag = 0;

   MPI_Status status;
   
   char buffer[100];

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);

   if(pid == 0){
      sprintf(msg,"hello");
      f = 1.0;
      n = 2;
      position = 0;
      MPI_Pack(msg, 10, MPI_CHAR, buffer, 100, &position, MPI_COMM_WORLD);
      MPI_Pack(&f, 1, MPI_FLOAT, buffer, 100, &position, MPI_COMM_WORLD);
      MPI_Pack(&n, 1, MPI_INT, buffer, 100, &position, MPI_COMM_WORLD);
      MPI_Send(buffer, 100, MPI_PACKED, 1, tag, MPI_COMM_WORLD);
   }
   if(pid == 1){
      MPI_Recv(buffer, 100, MPI_PACKED, 0, tag, MPI_COMM_WORLD, &status);

      position = 0;
      MPI_Unpack(buffer, 100, &position, msg, 10, MPI_CHAR, MPI_COMM_WORLD);
      MPI_Unpack(buffer, 100, &position, &f, 1, MPI_FLOAT, MPI_COMM_WORLD);
      MPI_Unpack(buffer, 100, &position, &n, 1, MPI_INT, MPI_COMM_WORLD);
      printf("%s %f %d \n", msg, f, n);
   }
   MPI_Finalize();
}

