# include <stdio.h>
# include <math.h>
# include <sys/time.h>
# include <stdlib.h>
# include "mpi.h"

int main(int argc, char *argv[])
{
   char msg[10];
   long N;
   int np, pid, tag = 0;
   int n;
   float f;
   MPI_Status status;

   MPI_Aint displacements[3], start_address, address;
   MPI_Datatype typelist[3];
   int block_lengths[3];
   

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);

   MPI_Datatype data_t;

   block_lengths[0] = 10;
   block_lengths[1] = 1;
   block_lengths[2] = 1;

   MPI_Address(msg, &start_address);
   displacements[0] = 0;
   MPI_Address(&f, &address);
   displacements[1] = address - start_address;
   MPI_Address(&n, &address);
   displacements[2] = address - start_address;

   typelist[0] = MPI_CHAR;
   typelist[1] = MPI_FLOAT;
   typelist[2] = MPI_INT;

   MPI_Type_struct(3, block_lengths, displacements, typelist, &data_t);
   MPI_Type_commit(&data_t);

   if(pid == 0){
      sprintf(msg,"hello");
      f = 1,0;
      n = 2;
      MPI_Send(msg, 1, data_t, 1, tag, MPI_COMM_WORLD);
   }
   if(pid == 1){
      MPI_Recv(msg, 1, data_t, 0, tag, MPI_COMM_WORLD, &status);
      printf("%s %f %d \n", msg, f, n);
   }
   MPI_Finalize();
}

