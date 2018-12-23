#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

int main(int argc, char* argv[]){
   int np, pid, inp, jnp;
   int source, dest;

   MPI_Comm grid_comm;
   int dim_sizes[2];
   int wrap_around[2];
   int coordinates[2];
   int reorder = 0;

   int direct = 1;
   int shift = 1;

   MPI_Status status;
   int pid_from, *pids_from, *ptr, i, j, k, tag;

   MPI_Init(&argc, &argv);

   MPI_Comm_rank(MPI_COMM_WORLD, &pid);
   MPI_Comm_size(MPI_COMM_WORLD, &np);

   inp = sqrt(np);
   jnp = np/inp;

   dim_sizes[0] = inp;
   dim_sizes[1] = jnp;

   wrap_around[0] = 1;
   wrap_around[1] = 1;

   MPI_Cart_create(MPI_COMM_WORLD, 2, dim_sizes, wrap_around, reorder, &grid_comm);
   MPI_Cart_coords(grid_comm, pid, 2, coordinates);

   printf("%d %d %d\n", pid, coordinates[0], coordinates[1]);

   direct = 0;
   shift = -1;
 
   MPI_Cart_shift(grid_comm, direct, shift, &source, &dest);

   printf("shifted - pid : %d  cords : %d %d source : %d dest : %d\n", pid, coordinates[0], coordinates[1], source, dest);

   MPI_Send(&pid, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
   MPI_Recv(&pid_from, 1, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);

   MPI_Finalize();
}
