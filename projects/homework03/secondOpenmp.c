int printf(char *, ...);

main()
{

   int my_proc;
   int sum = 0;

   # pragma omp parallel private(my_proc) shared(sum)
   {
      printf("--%d\n", omp_get_num_threads());
      my_proc = omp_get_thread_num();
      printf("%d\n",my_proc);
      sum += my_proc;
      printf("---%d\n",sum);
   }

   printf("%d\n",sum);

}
