int printf(char *, ...);

main()
{

   int nproc, myproc, i;

   # pragma omp parallel private(myproc, i)
   {
      nproc = omp_get_num_threads();
      myproc = omp_get_thread_num();
      printf("--%d %d\n", nproc, myproc);

      #pragma omp for
      for(i = 0; i < 16; i++)
      {

         printf("%d %d\n",myproc,i);
      }
   }


}
