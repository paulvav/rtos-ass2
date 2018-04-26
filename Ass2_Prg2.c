#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>

#define SHMSZ 8192

int shmid;
key_t key = 6000;
double *shm, *s;


main()
{

	if ((shmid = shmget(key, SHMSZ, 0666)) < 0) 
	{
        	perror("shmget");
  
  	}

	if ((shm = shmat(shmid, NULL, 0)) == (double *)-1) 
	{
        	perror("shmat");
    	}
	
	s=shm;
	printf("Last Runtime was: %f seconds\n", *shm);
}
