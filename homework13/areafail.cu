# include <stdio.h>
# include <math.h>
# include <sys/time.h>

# define N 1000000
# define RADIUS 100
# define THREADS 32

__global__ void QuarterAreaOfCircle ( float *area ){

   //int i = blockDim.x*blockIdx.x+threadIdx.x;
   float blockStartX;
   float XofEachIdx, dx;

   __shared__ float segmentArea[THREADS];

   // x starting value of each block
   blockStartX = ((float)blockIdx.x * (float)(RADIUS/gridDim.x));

   // increasing value of x
   dx = (float)RADIUS/(float)N;

   // X value of each thread
   XofEachIdx = blockStartX + ((float)threadIdx.x * dx);

   // calculate segment area
   segmentArea[threadIdx.x] = sqrt(fabs((float)RADIUS*(float)RADIUS-XofEachIdx*XofEachIdx)) * dx;
   __syncthreads();
   // reduce 32 threads area to one
   for(unsigned int s = 0; s < threadIdx.x; s++){
      area[blockIdx.x] += segmentArea[s];
      __syncthreads();
   }
}

int main(int argc, char *argv[])
{
   float *reduceArea_d, reduceArea[N/THREADS], Area = 0;
   int i;

   dim3 dimBlock(THREADS);
   dim3 dimGrid(N/dimBlock.x);

   cudaMalloc( (void**) &reduceArea_d, sizeof(float) * dimGrid.x );

   QuarterAreaOfCircle<<<dimGrid, dimBlock>>>(reduceArea_d);

   cudaMemcpy(reduceArea, reduceArea_d, sizeof(float)*dimGrid.x, cudaMemcpyDeviceToHost);

   for(i = 0; i < dimGrid.x; i++){
     Area += reduceArea[i]; 
     //printf("reduced area : %5.10f , grid : %d,  area : %5.10f\n", reduceArea[i], i, Area);
   }

   printf("area : %5.10f\n",Area*4);
   cudaFree(reduceArea_d);
}
