# include <stdio.h>
# include <stdlib.h>

#define M 8
#define N 9

#define THREAD_X 2
#define THREAD_Y 3

#define A(i,j) A[i*N+j]
#define B(i,j) B[i*N+j]
#define C(i,j) C[i*N+j]

__global__ void matAdd(int *A, int *B, int *C){
   int i = blockDim.x * blockIdx.x + threadIdx.x;
   int j = blockDim.y * blockIdx.y + threadIdx.y;

   A(i,j) = B(i,j) + C(i,j);
}

int main(){
   int A[M][N], B[M][N], C[M][N];
   int *A_d, *B_d, *C_d;
   int i, j;

   dim3 dimBlock(THREAD_X, THREAD_Y);
   dim3 dimGrid(M/THREAD_X, N/THREAD_Y);

   cudaMalloc((void**)&A_d, M*N*sizeof(int));
   cudaMalloc((void**)&B_d, M*N*sizeof(int));
   cudaMalloc((void**)&C_d, M*N*sizeof(int));

   for(i = 0; i < M; i++){
      for(j = 0; j < N;j++){
         B[i][j] = i*j;
         C[i][j] = i+j;
         //printf("%d ",B[i][j]);
      }
      //printf("\n");
   }

   cudaMemcpy(A_d, A, sizeof(int)*M*N, cudaMemcpyHostToDevice);
   cudaMemcpy(B_d, B, sizeof(int)*M*N, cudaMemcpyHostToDevice);
   cudaMemcpy(C_d, C, sizeof(int)*M*N, cudaMemcpyHostToDevice);

   matAdd<<<dimGrid, dimBlock>>>(A_d, B_d, C_d);

   cudaMemcpy(A, A_d, sizeof(int)*M*N, cudaMemcpyDeviceToHost);
   
   for(i = 0; i < M; i++){
      for(j = 0; j < N;j++){
         printf("%d ", A[i][j]);
      }
      printf("\n");
   }

   cudaFree(A_d);
   cudaFree(B_d);
   cudaFree(C_d);
}
