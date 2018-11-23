#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

#define Ng 12

main(int argc, char* argv[])
{

   float Ag[Ng][Ng], Bg[Ng][Ng], Cg[Ng][Ng], *A, *A2, *B, *C;
   float *ap, *ap2, *bp, *cp;
   int N, i, j, n;

   int np, pid, inp, jnp, source, dest;
   int direct, shift, next, reorder;

   MPI_Comm grid_comm, row_comm;
   MPI_Datatype vector_t;
   int dim_sizes[2], wrap_around[2], coord[2];
   MPI_Status status;
   int tag;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);
   MPI_Comm_size(MPI_COMM_WORLD, &np);

   inp = jnp = sqrt(np);
   N = Ng/inp;
   dim_sizes[0] = inp;
   dim_sizes[1] = inp;
   reorder = 0;
   direct = 0;

   wrap_around[0] = 1;
   wrap_around[1] = 1;
   if (pid == 0) {
      n = 1;
      for (i=0; i<Ng; i++) {
         for (j=0; j<Ng; j++) {
            Ag[i][j] = n*0.01;
            Bg[i][j] = n++*0.02;
            Cg[i][j] = 0.0;
         }
      }
      printf("matrix A------------------------\n");
      print(Ng, Ng, Ag);
      printf("matrix B------------------------\n");
      print(Ng, Ng, Bg);
      mmult(Ng, Ng, Ng, Ag, Bg, Cg);
      printf("sequential multiplication ------\n");
      print(Ng, Ng, Cg);
   }

   A = (float*)malloc(sizeof(float)*N*N);
   A2 = (float*)malloc(sizeof(float)*N*N);
   B = (float*)malloc(sizeof(float)*N*N);
   C = (float*)malloc(sizeof(float)*N*N);

   // MPI data type for distribution and composition
   MPI_Type_vector(N,N,Ng,MPI_FLOAT, &vector_t);
   MPI_Type_commit(&vector_t);

   if (pid == 0) {
      // copy to local A on p0
      ap = A;
      bp = B;
      for (i=0; i<N; i++)
         for (j=0; j<N; j++) {
            *ap++ = Ag[i][j];
            *bp++ = Bg[i][j];
         }
      
      // distribute Ag and Bg
      for (i=1; i<np; i++) {
         MPI_Send(&(Ag[N*(i/inp)][N*(i%inp)]), 1, vector_t, i, tag, MPI_COMM_WORLD);
         MPI_Send(&(Bg[N*(i/inp)][N*(i%inp)]), 1, vector_t, i, tag, MPI_COMM_WORLD);
      }
   }
   else {
      MPI_Recv(A, N*N, MPI_FLOAT, 0, tag,MPI_COMM_WORLD, &status);
      MPI_Recv(B, N*N, MPI_FLOAT, 0, tag,MPI_COMM_WORLD, &status);
   }

   MPI_Cart_create(MPI_COMM_WORLD,2,dim_sizes, wrap_around, reorder, &grid_comm);
   MPI_Cart_coords(grid_comm, pid, 2, coord);
   
   // cartesian communicator for shift B north
   MPI_Cart_shift(grid_comm,direct, 1, &source, &dest);

   // split communicator for broadcast Ai to row
   MPI_Comm_split(grid_comm,pid/2,pid, &row_comm);

   // initialize C
   cp = C;
   for (i=0; i<N*N; i++)
      *cp++ = 0.0;

   next = coord[0];
   for (i=0; i<inp; i++) {
      //step1: broadcast diag Ai to row
      ap = A;
      ap2 = A2;
      for (j=0; j<N*N; j++)
         *ap2++ = *ap++;
      MPI_Bcast(A2, N*N, MPI_FLOAT, (next++)%jnp, row_comm);
 
      //step2: C += A * B
      mmult(N, N, N, A2, B, C);

      // step3: shift B north
      MPI_Send(B, N*N, MPI_FLOAT, dest, tag, MPI_COMM_WORLD);
      MPI_Recv(B, N*N, MPI_FLOAT, source, tag, MPI_COMM_WORLD, &status);
   }
   // compose Cg
   if (pid != 0)
      MPI_Send(C, N*N, MPI_FLOAT, 0, tag, MPI_COMM_WORLD);
   else {
      // copy to local Ag on p0
      cp = C;
      for (i=0; i<N; i++)
         for (j=0; j<N; j++)
            Cg[i][j] = *cp++;
      for (i=1; i<np; i++)
         MPI_Recv(&(Cg[N*(i/inp)][N*(i%inp)]), 1, vector_t, i, tag, MPI_COMM_WORLD, &status);

   }
   if (pid == 0) {
      printf("parallel multiplication ------\n");
      print(Ng, Ng, Cg);
   }

   free(A);
   free(A2);
   free(B);
   free(C);

   MPI_Finalize();
}
