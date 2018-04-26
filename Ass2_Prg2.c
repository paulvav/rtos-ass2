#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SHMSZ 8192
#define MEM_NAME "shared"

int shmid;
key_t key = 6000;
double *shm, *s;


main()
{

	if ((shmid = shm_open(MEM_NAME,O_RDONLY ,0666)) < 0) 
        	perror("shmget");
  

	if ((shm = mmap(0, SHMSZ, PROT_READ, MAP_SHARED, shmid, 0)) == (double *)-1) 
	{
        	perror("shmat");
    	}
	
	printf("Last Runtime was: %f seconds\n", *shm);
}
