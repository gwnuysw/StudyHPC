# include <stdio.h>
# include <math.h>
# include <sys/time.h>

# define RADIUS 100
int main(int argc, char *argv[])
{
   long N,i;
   double dx, area, x;
   int start, stop, nproc, myproc;
   char *c;

   N = 5000000000;
   if(argc == 2) N = (long)strtol(argv[1], &c, 10);
   start = millisecond();
   
   dx = (double)RADIUS/(double)N;

   area = 0.0;
   #pragma omp parallel for reduction(+:area)
   for(i = 1; i <= N; i++)
   {
      x = dx * i;
      area += sqrt(fabs((double)RADIUS*(double)RADIUS-x*x)) * dx;
   }
   stop = millisecond();
   printf("area : %5.10lf, x : %lf\n", myproc, area*4, x);
   printf("excution time : %d milliseconds\n",stop-start);
}
