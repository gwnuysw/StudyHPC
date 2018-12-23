#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "smemory.h"

main(int argc, char*argv[])
{
   int shm_id;
   void *shm_addr;
   SHM *shm;
   int key;

   int sum = 0;
   int presum = 0;
   int data = 0;
   if (argc != 2) {
      printf("usage: %s key\n", argv[0]);
      exit(1);
   }

   key = atoi(argv[1]);

/*
   FILL IN THIS BLANK
*/
   shm_id = shmget(key, sizeof(int), IPC_CREAT|0666);
   shm_addr = (void*)shmat(shm_id,0,0);
   shm = (SHM*) shm_addr;

   shm->sum = 0;
   printf("sum : %d\n",sum);
   while(presum <= sum){
      presum = sum;
      shm->s = 1;  //yeild

      while(!shm->s); //wait athor processor

      shm->s = 0; //lock semaphore

      //access shared memory
      if(shm->sum)
      {
         sum += shm->sum;
         printf("sum : %d\n", sum);
         shm->sum = 0;
      }

      shm->s = 1; //unlock semaphore
   }
   shmdt((void*)shm_addr);
   shmctl(shm_id, IPC_RMID, 0);

   exit(0);
}
