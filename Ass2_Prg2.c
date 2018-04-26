#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

//Memory size for shared memory
#define SHMSZ 8192

//Name of shared memory - Idenfier
#define MEM_NAME "shared"

//initialising shared memory id
int shmid;

//initialising pointer for memory
double *shm;


main()
{
	//opening memor location - check for errors
	if ((shmid = shm_open(MEM_NAME,O_RDONLY ,0666)) < 0) 
        	perror("shmget");
  
	//fetching contents of memory and pushing it to shm
	if ((shm = mmap(0, SHMSZ, PROT_READ, MAP_SHARED, shmid, 0)) == (double *)-1) 
        	perror("shmat");
 
	//print memory contents
	printf("Last Runtime was: %f seconds\n", *shm);
}
