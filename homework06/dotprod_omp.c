# include <stdio.h>
# include <math.h>
# include <sys/time.h>
# include <stdlib.h>

# define N 500000000
int millisecond(){
   struct timeval tv;
   gettimeofday(&tv, NULL);
   return tv.tv_sec * 1000 + tv.tv_usec/1000;
};

int main(int argc, char *argv[])
{
   double *A, *B, dotprod, MFLOPS;
   int start, stop, excution;
   long i;

   start = millisecond();
   
   A = malloc(sizeof(double)*N);
   B = malloc(sizeof(double)*N);

   #pragma omp parallel private(i)
   {
      #pragma omp for
      for(i = 0; i < N; i++)
      {
         A[i] = i * 0.000000000001;
         B[i] = i * 0.000000000002 + 3.0; 
      }
   }
   dotprod = 0.0;
   #pragma omp parallel for reduction(+:dotprod)
   for(i = 0; i < N; i++)
      dotprod += A[i] * B[i];

   stop = millisecond();
   excution = stop - start;
   MFLOPS = 5.0 * N /  (1000.0 * (double)excution); //mega * excution but adjust time scale

   printf("dotpord : %5.10lf, MFLOPS : %5.10lf\n", dotprod, MFLOPS);
   printf("excution time : %d milliseconds\n",excution);
   free(A);
   free(B);
}
