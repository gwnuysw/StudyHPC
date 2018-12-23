# include <stdio.h>
# include <math.h>
# include <sys/time.h>
# include <stdlib.h>
# include "mpi.h"

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
   int np, pid, tag = 0;
   int start, stop;
   char *c;
   double Local_area, * GatherArea, x;
   double area, dx;
   MPI_Status status;

   N = 5000000000;
   if(argc == 2) N = (long)strtol(argv[1], &c, 10);
   
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &np);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);

   GatherArea = (double *)malloc(sizeof(double)*np-1);

   start = millisecond();

   //start each calcuation
   Local_area = 0.0;
   dx = (double)RADIUS/(double)N;
   x = (double)pid * ((double)RADIUS/(double)np); 
   for(i = 0; i < N/np; i++)
   {
      x += dx;
      Local_area += sqrt(fabs((double)RADIUS*(double)RADIUS-x*x)) * dx;
   }
   //end each calculation
   MPI_Gather(&Local_area, 1, MPI_DOUBLE, GatherArea, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   if(pid == 0){
      area = 0.0;
      for(i = 0; i < np; i++){
        printf("GatherArea[i] : %lf\n", GatherArea[i]); 
      }
      for(i = 0; i < np; i++){
        area += GatherArea[i]; 
      }
      stop = millisecond();
      printf("area : %5.10lf\n", area*4);
      printf("excution time : %d milliseconds\n",stop-start);
   }


   free(GatherArea);//MPI execution error I can't figureout
   MPI_Finalize();
}
