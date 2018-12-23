# include <stdio.h>
# include <math.h>
# include <sys/time.h>

# define RADIUS 100

int millisecond()
{
   struct timeval tv;
   gettimeofday(&tv, NULL);
   return tv.tv_sec * 1000 + tv.tv_usec/1000;
};
int main(int argc, char *argv[])
{
   long N,i;
   double dx, area, x, np, pid;
   int start, stop;
   char *c;

   N = 5000000000;
   if(argc == 2) N = (long)strtol(argv[1], &c, 10);
   start = millisecond();
   
   dx = (double)RADIUS/(double)N;

   area = 0.0;
   #pragma omp parallel private(i, x, pid) shared(area, dx, np)
   {
      np = (double)omp_get_num_threads();
      pid = (double)omp_get_thread_num();
      x = pid * ((double)RADIUS/np); 
      printf("x : %lf, pid : %lf, np : %lf, N :%f\n", x, pid, np, N);
      #pragma omp for reduction(+:area)
      for(i = 0; i < N; i++)
      {
         x += dx;
         area += sqrt(fabs((double)RADIUS*(double)RADIUS-x*x)) * dx;
      }
      printf("i : %d\n", i);
   }
   stop = millisecond();
   printf("area : %5.10lf\n", area*4);
   printf("excution time : %d milliseconds\n",stop-start);
}
