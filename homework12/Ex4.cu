#include <stdio.h>

#define X 9
#define Y 8

#define THREAD_X 3
#define THREAD_Y 2

#define A(i, j) A[i*Y + j]

__global__ void index(int *A){
   int i = blockDim.x * blockIdx.x + threadIdx.x;
   int j = blockDim.y * blockIdx.y + threadIdx.y;
   
   //A(i, j) = threadIdx.x;
   A(i, j) = threadIdx.y;
   //A(i ,j) = blockIdx.y;
}
int main(){
   int A[X][Y], *A_d;
   int i, j;

   dim3 dimBlock(THREAD_X, THREAD_Y);
   dim3 dimGrid(X/THREAD_X, Y/THREAD_Y);

   cudaMalloc((void**)&A_d, sizeof(int)*X*Y);

   for(i = 0; i < X; i++)
      for(j = 0; j < Y; j++)
         A[i][j] = -1;

   cudaMemcpy(A_d, A, sizeof(int)*X*Y, cudaMemcpyHostToDevice);

   index<<<dimGrid, dimBlock>>>(A_d);

   cudaMemcpy(A, A_d, sizeof(int)*X*Y, cudaMemcpyDeviceToHost);

   for(i = 0; i < X; i++){
      for(j = 0; j < Y; j++){
         printf("%d ", A[i][j]);
      }
      printf("\n");
   }
   cudaFree(A_d);
}
