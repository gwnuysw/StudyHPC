# include <stdio.h>
# include <math.h>
# include <sys/time.h>

# define N 1000000
# define RADIUS 100
# define THREADS 32

__global__ void QuarterAreaOfCircle (  float *area , float *start, float *end){

   //int i = blockDim.x*blockIdx.x+threadIdx.x;
   int i = 0;
   float threadStartX;
   float x, dx;

   float segmentArea;

   // x starting value of each block
   
   threadStartX = ((float)RADIUS/(float)blockDim.x);
   start[threadIdx.x] = (float)threadIdx.x * threadStartX;
   x = start[threadIdx.x];
   end[threadIdx.x] = x;

   // increasing value of x
   dx = (float)RADIUS/(float)N;

   // calculate segment area
   for(i = 0; i < ((float)N/(float)blockDim.x); i++){
      x += dx;
      segmentArea += sqrt(fabs((float)RADIUS*(float)RADIUS-x*x)) * dx;
   }
   end[threadIdx.x] = dx;
   area[threadIdx.x] = segmentArea;
}

int main(int argc, char *argv[])
{
   float *reduceArea_d, reduceArea[THREADS], Area = 0;
   float *start_d, start[THREADS];
   float *end_d, end[THREADS];
   int i;

   dim3 dimBlock(THREADS);
   dim3 dimGrid(1);

   for( i = 0; i < dimBlock.x; i++){
      reduceArea[i] = 0;
   }

   cudaMalloc( (void**) &reduceArea_d, sizeof(float) * THREADS );
   cudaMalloc( (void**) &start_d, sizeof(float) * THREADS );
   cudaMalloc( (void**) &end_d, sizeof(float) * THREADS );

   QuarterAreaOfCircle<<<dimGrid, dimBlock>>>(reduceArea_d, start_d, end_d);

   cudaMemcpy(reduceArea, reduceArea_d, sizeof(float)*dimBlock.x, cudaMemcpyDeviceToHost);
   cudaMemcpy(start, start_d, sizeof(float)*dimBlock.x, cudaMemcpyDeviceToHost);
   cudaMemcpy(end, end_d, sizeof(float)*dimBlock.x, cudaMemcpyDeviceToHost);

   for(i = 0; i < dimBlock.x; i++){
     Area += reduceArea[i]; 
     printf("reduced area : %5.10f , grid : %d,  area : %5.10f, sart : %5.10f, end : %5.10f \n", reduceArea[i], i, Area, start[i], end[i]);
   }

   printf("area : %5.10f \n",Area*4);
   cudaFree(reduceArea_d);
   cudaFree(start_d);
   cudaFree(end_d);
}
