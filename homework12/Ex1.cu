#include <stdio.h>

#define X 12

#define THREAD_X 4

__global__ void index(int *A){
   int i = blockDim.x*blockIdx.x+threadIdx.x;
   //int i = threadIdx.x;

   A[i] = i;
//   A[i] = gridDim.x;
   //A[i] = blockDim.x;
//   A[i] = threadIdx.x;
}

int main(){
   int A[X], *A_d;
   int i;

   //dim3 dimBlock(THREAD_X, THREAD_Y);
   //dim3 dimGrid(X/THREAD_X,Y/THREAD_Y);

   cudaMalloc((void**)&A_d, sizeof(int)*X);

   for(i = 0; i < X; i++){
      A[i] = -1;
   }

   cudaMemcpy(A_d, A, sizeof(int)*X, cudaMemcpyHostToDevice);

   index<<<12/4, 4>>>(A_d);
   //index<<<dimGrid, dimBlock>>>(A_d);


   cudaMemcpy(A, A_d, sizeof(int)*X, cudaMemcpyDeviceToHost);

   for(i=0; i < X; i++){
      printf("%d ",A[i]);
   }
   printf("\n");
   cudaFree(A_d);
}
