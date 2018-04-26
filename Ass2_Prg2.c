#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>

#define SHMSZ 27

int shmid;
key_t key = 5678;
char *shm, *s;


main()
{

	if ((shmid = shmget(key, SHMSZ, 0666)) < 0) 
	{
        	perror("shmget");
  
  	}

	if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) 
	{
        	perror("shmat");
    	}
	
	s=shm;
	printf("%s\n", s);
}
