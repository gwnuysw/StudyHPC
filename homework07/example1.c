# include <mpi.h>
main(int argc, char* argv[])
{
   MPI_Init(&argc, &argv);
   system("hostname");
   MPI_Finalize();
}
