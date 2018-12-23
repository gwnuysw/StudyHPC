#include <stdio.h>

#define N 10000
#define THREAD_X 4

__global__ void index(float *A, float *B, float *C){
   int i = blockDim.x*blockIdx.x+threadIdx.x;

   C[i] = A[i] + B[i];
}
__global__ void inIt(float *A, float *B){
   int i = blockDim.x*blockIdx.x+threadIdx.x;

   A[i] = i*2;
   B[i] = N-i;
}

int main(){
   float A[N], *A_d;
   float B[N], *B_d;
   float C[N], *C_d;
   int i;


   dim3 dimBlock(THREAD_X);
   dim3 dimGrid(N/THREAD_X);

   cudaMalloc((void**)&A_d, sizeof(int)*N);
   cudaMalloc((void**)&B_d, sizeof(int)*N);
   cudaMalloc((void**)&C_d, sizeof(int)*N);


   cudaMemcpy(A_d, A, sizeof(int)*N, cudaMemcpyHostToDevice);
   cudaMemcpy(B_d, B, sizeof(int)*N, cudaMemcpyHostToDevice);

   inIt<<<dimGrid, dimBlock>>>(A_d, B_d);

   cudaMemcpy(C_d, C, sizeof(int)*N, cudaMemcpyHostToDevice);

   index<<<dimGrid, dimBlock>>>(A_d, B_d, C_d);

   cudaMemcpy(A, A_d, sizeof(int)*N, cudaMemcpyDeviceToHost);
   cudaMemcpy(B, B_d, sizeof(int)*N, cudaMemcpyDeviceToHost);
   cudaMemcpy(C, C_d, sizeof(int)*N, cudaMemcpyDeviceToHost);

   for(i = 0; i < N; i++){
      printf("%f ",C[i]);
   }
   cudaFree(B_d);
   cudaFree(C_d);
   cudaFree(A_d);
}
