#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include "smemory.h"

main(int argc, char*argv[])
{
   int key;
   SHM *shm;
   void *shm_addr;
   int shm_id;
   int data;

   if (argc != 3) {
      printf("usage: %s key data\n", argv[0]);
      exit(1);
   }

   key = atoi(argv[1]);
   data = atoi(argv[2]);

/*
   FILL IN THIS BLANK
*/
   shm_id = shmget(key, sizeof(int), 0666);
   shm_addr = (void *)shmat(shm_id, 0, 0);
   shm = (SHM*)shm_addr;

   shm->s = 1;
   while(!shm->s);
   shm->s = 0;
   shm->sum = data;
   shm->s = 1;

   printf("memory access! %d\n", shm->sum);
   exit(0);
}
