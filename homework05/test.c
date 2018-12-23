# include <stdio.h>
# include <math.h>
# include <sys/time.h>

# define RADIUS 1
int main(int argc, char *argv[])
{
   long N,i;
   double dx, area, x;
   int start, stop, nproc, myproc;
   char *c;

   N = 5000000000;
   if(argc == 2) N = (long)strtol(argv[1], &c, 10);
   start = millisecond();
   
   dx = 0.3;

   area = 0.0;
   //x = 0.0;
   #pragma omp parallel private(myproc, i)
   {
      nproc = omp_get_num_threads();
      myproc = omp_get_thread_num();
      printf("omp_get_num_threads : %d, omp_get_thread_num : %d\n", nproc, myproc);
      #pragma omp for
     // #pragma omp parallel for reduction(+:area)
      for(i = 0; i < N; i++){
         area = area + 1;
     //    printf("omp_get_num_threads : %d, omp_get_thread_num : %d, area: %d\n", nproc, myproc, area);
      }
   }
   stop = millisecond();
   printf("area : %5.10f\n",area);
   printf("excution time : %d milliseconds\n",stop-start);
}
