# include <stdio.h>
# include <math.h>
# include <sys/time.h>
# include "millisecond.h"

# define RADIUS 1

int main(int argc, char *argv[])
{
   long N, i;
   double dx, x, area;
   double start, stop;
   char *c;

   N = 10000;
   if(argc == 2) N = (long)strtol(argv[1], &c, 10);
   printf("%ld\n",N);

   start = millisecond();
   
   area = 0.0;
   dx = (double)RADIUS/(double)N;

   x = 0.0;
   for(i = 1; i < N ; i++){
      x += dx;
      area += sqrt(fabs((double)RADIUS*(double)RADIUS-x*x)) * dx;
   }
   stop = millisecond();
   printf("area : %5.10f\n",area*4);
   printf("excution time : %lf milliseconds\n",stop-start);
}
