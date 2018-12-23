#include <stdio.h>

#define N 1000
#define THREAD_X 4

__global__ void index(float *A, float *B){
   int i = blockDim.x*blockIdx.x+threadIdx.x;
   //int i = threadIdx.x;
   float X = 1.23;
   float Y = 2.34 ;

   B[i] = A[i]*X + Y;
}

int main(){
   float A[N], *A_d;
   float B[N], *B_d;
   int i;

   dim3 dimBlock(THREAD_X);
   dim3 dimGrid(N/THREAD_X);

   for(i = 0 ; i < N; i++){
      A[i] = i*2;
   }

   cudaMalloc((void**)&A_d, sizeof(int)*N);
   cudaMalloc((void**)&B_d, sizeof(int)*N);

   cudaMemcpy(A_d, A, sizeof(int)*N, cudaMemcpyHostToDevice);
   cudaMemcpy(B_d, B, sizeof(int)*N, cudaMemcpyHostToDevice);

   index<<<dimGrid, dimBlock>>>(A_d, B_d);

   cudaMemcpy(A, A_d, sizeof(int)*N, cudaMemcpyDeviceToHost);
   cudaMemcpy(B, B_d, sizeof(int)*N, cudaMemcpyDeviceToHost);

   for(i = 0; i < N; i++){
      printf("%f ",B[i]);
   }

   cudaFree(A_d);
}
