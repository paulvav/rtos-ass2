/*
RTOS assignment 2 - UTS
Program to read from user specified Input file and Write to user specified Output file.
The purpose of this program is to demonstrate the functionality of Threads and Semaphores in C.
By Paul Vavich 11685726
*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

//Array for pipe
int fd[2];

/*Declaring three semaphores */
sem_t sread, swrite, sjustify;

//Thread ID
pthread_t tid1,tid2, tid3;

//Set of thread attributes			
pthread_attr_t attr;				

//Keyword indicating end of header, eg. when document will start writing
char header_end[] = "end_header\n";    

//NOTE: This is the maximum size per line of the input File
#define MAX_BUF 50

//struct to pass to each thread
struct Parser
{
	char buf[MAX_BUF];
	FILE * inputF;
	FILE * outputF;

} thread_data;

void *A(void *args);/*threads call this function*/
void *B(void *args);/*threads call this function*/
void *C(void *args);/*threads call this function*/

int main(int argc, char*argv[])
{
	//Find Start time to calculate run time
        clock_t begin = clock();
	
	//Argument checker - User must input two file names
	if(argc!=3)
	{
		fprintf(stderr,"Usage: Inputfile OutPutfile");
		return -1;
	}

	//Declare struct
	struct Parser thread_data;
  	
	//Add Files to struct
	thread_data.inputF = fopen(argv[1], "r");
	thread_data.outputF = fopen(argv[2], "w");
    
     	/* Get the default attributes */
	pthread_attr_init(&attr);

   	//Initialise Pipe
	if (pipe(fd) == -1)
   		printf("PIPE-ERROR");
 
  	/*get the default attributes*/
	pthread_attr_init(&attr);
  
  	/*create the threads */
	pthread_create(&tid1,&attr,A,(void *)&thread_data);
	pthread_create(&tid2,&attr,B,(void *)&thread_data);  
	pthread_create(&tid3,&attr,C,(void *)&thread_data);

	//wait for thread A to end	
	pthread_join(tid1,NULL);
	int i = 0;
	while(i < 100000000)
	{
	i++;
	}

	clock_t end = time();
	double time_spent = (double)(end-begin)/CLOCKS_PER_SEC;
	printf("End time %lf", (double)end);
	
	
}

void *A(void *args)
{
	//String to hold contents of file
	char lineFromFile[100];
	
	//Declaring local pointer of struct
	struct Parser *tdata = (struct Parser *) args;
  	
	//For every line in input file...
	while(fgets(lineFromFile,1000,tdata->inputF))
	{
		//write line of file into the pipe
		if(write(fd[1], lineFromFile, strlen(lineFromFile)+1) != strlen(lineFromFile)+1)
			printf("Pipe Error: Write Failed");
		
		//send semaphore that pipe is ready to be read	  	
		sem_post(&sread);
		
		//wait until next threads are ready
	  	sem_wait(&swrite);  
	}
	//input file has been completey read	
	
	//close output file
	fclose(tdata->outputF);

	//clost other threads 
	pthread_cancel(tid2);
	pthread_cancel(tid3);
	printf("A:%f", (double)clock());
}

/*The thread will begin control in this function*/
void *B(void *args)
{
	//Declaring local pointer of struct
	struct Parser *tdata = (struct Parser *) args;
	
	while(1)
	{	//wait for Thread A to put data in pipe
  		sem_wait(&sread);

		//Read Data from pipe
		if(read(fd[0], tdata->buf, MAX_BUF) < 0)
			printf("Pipe Error: Read Failed");
		//Semaphore to start next thread
		sem_post(&sjustify); 	 
	}
}

void *C(void *args)
{
	//Declaring local pointer of struct
	struct Parser *tdata = (struct Parser *) args;
	
	//Flag indicating if the header has been detected as over	
	int found_header = 0; 
	 
	while(1)
	{	
		//wait for thread B to send complete signal
		sem_wait(&sjustify);

		//Write to file only if the end header has been passed
     		if(found_header)		
			//Write to file
       			fprintf(tdata->outputF,tdata->buf);
      
		//If End header has been found
   		if(!strcmp(tdata->buf,header_end))	
			//set header flag       			
			found_header = 1;

       		//Send signal that write is done for Thread A
		sem_post(&swrite);
      	}
}


