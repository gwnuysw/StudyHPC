#include <stdio.h>

#define N 16
#define THREADS 32

__global__ void max_kernel(float *A, float * max){
   __shared__ float smax[THREADS];

   smax[threadIdx.x] = A[threadIdx.x];

   for(unsigned int s = blockDim.x/2; s > 0; s>>=1){
      if(threadIdx.x < s){
         if(smax[threadIdx.x] < smax[threadIdx.x+s]){
            smax[threadIdx.x] = smax[threadIdx.x+s];
            __syncthreads();
         }
      }
   }
   if(threadIdx.x == 0){
      *max = smax[threadIdx.x];
   }
}
int main(){
   float A[N], max;
   float *A_d, *max_d;

   int i;
   dim3 dimBlock(THREADS);
   dim3 dimGrid((N+dimBlock.x-1)/dimBlock.x);
   max = -10000.0;
   for(i = 0; i < N;i++){
      A[i] = rand()/(float)(1<<30);
      printf("%5.2f ", A[i]);
      if(A[i] > max)
         max = A[i];
   }
   printf("\nCPU -- %5.2f\n", max);

   cudaMalloc((void**) &A_d, sizeof(float)*N);
   cudaMalloc((void**) &max_d, sizeof(float));
   cudaMemcpy(A_d, A, sizeof(float)*N, cudaMemcpyHostToDevice);

   max_kernel <<<dimGrid, dimBlock>>> (A_d, max_d);

   max = -1.0;
   cudaMemcpy(&max, max_d, sizeof(float), cudaMemcpyDeviceToHost);

   printf("CPU--%5.2f \n", max);

   cudaFree(A_d);
   cudaFree(max_d);
}

