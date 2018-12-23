#include <stdio.h>

void print(int M, int N, float A[M][N])
{
   int i, j, k;

   for (i=0; i<M; i++) {
      for (j=0; j<N; j++)
         printf("%5.2f ", A[i][j]);
      printf("\n");
   }
}

void mmult(int L, int M, int N, float A[L][M], float B[M][N], float C[L][N])
{
   int i, j, k;

   for (i=0; i<L; i++) {
      for (j=0; j<N; j++) {
         for (k=0; k<M; k++) {
            C[i][j] += A[i][k] * B[k][j];
         }
      }
   }
}
