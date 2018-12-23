# define N 100

int printf(char *, ...);

int main()
{
   int sum = 0, i;
   #pragma omp parell for reductino(+:sum)
   for(i=0;i<N;i++)
      sum += i;

   printf("%d\n",sum);
}
