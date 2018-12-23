#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define Ng 12

main(int argc, char* argv[])
{

   float Ag[Ng][Ng], Bg[Ng][Ng], Cg[Ng][Ng], *A, *A2, *B, *C;
   float *ap, *ap2, *bp, *cp;
   int N, i, j, n;

   int np, pid, inp, jnp, source, dest;
   int direct, shift, next;

   int dim_sizes[2], wrap_around[2], coord[2], reorder;
   int tag;
   np = 4;

   inp = jnp = sqrt(np);
   N = Ng/inp;

      n = 1;
      for (i=0; i<Ng; i++) {
         for (j=0; j<Ng; j++) {
            Ag[i][j] = n*0.01;
            Bg[i][j] = n++*0.02;
            Cg[i][j] = 0.0;
         }
      }
      mmult(Ng, Ng, Ng, Ag, Bg, Cg);
      printf("A sequential multiplication ------\n");
      print(Ng, Ng, Ag);
      printf("B sequential multiplication ------\n");
      print(Ng, Ng, Bg);
   A = (float*)malloc(sizeof(float)*N*N);
   A2 = (float*)malloc(sizeof(float)*N*N);
   B = (float*)malloc(sizeof(float)*N*N);
   C = (float*)malloc(sizeof(float)*N*N);

   // MPI data type for distribution and composition

      // copy to local A on p0
      ap = A;
      bp = B;
    printf("Ag initialized\n");
      for (i=0; i<N; i++){
         for (j=0; j<N; j++) {
            *ap++ = Ag[i][j];
            *bp++ = Bg[i][j];
            printf("%5.2f ",Ag[i][j]);
         }
         printf("\n");
      }
      // distribute Ag and Bg
      printf("A\n");
      for(i = 1; i < np; i++){
         printf("%5.2f\n", Ag[N*(i/inp)][N*(i%inp)]);
      }
      printf("B\n");
      for(i = 1; i < np;i++){
         printf("%5.2f\n", Bg[N*(i/inp)][N*(i%inp)]);
      }
   free(A);
   free(A2);
   free(B);
   free(C);
   return 0;
}
