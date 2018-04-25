#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <string.h>


int sum;/*this data is shared by the threads*/
/* The mutex lock */
int fd[2];
pthread_mutex_t mutex;
/* the semaphores */
sem_t one, two, sread, swrite, justify;
pthread_t tid1,tid2, tid3;       //Thread ID
pthread_attr_t attr; //Set of thread attributes

void *A(void *param);/*threads call this function*/
void *B(void *param);/*threads call this function*/
void *C(void *param);

void initializeData();
FILE * file;

#define MAX_BUF 50
char buf[MAX_BUF];



int main(int argc, char*argv[])
{
 // pthread_t tid;/*the thread identifier*/
 // pthread_attr_t attr;/*set of thread attributes*/

  if(argc!=2){
	fprintf(stderr,"usage: a.out <integer value>\n");
	return -1;
  }
  if(atoi(argv[1])<0){
	fprintf(stderr,"%d must be >=0\n",atoi(argv[1]));
	return -1;
  }
  initializeData();
 
  /*get the default attributes*/
  pthread_attr_init(&attr);
  
  /*create the thread 1*/
  pthread_create(&tid1,&attr,A,argv[1]);
  
  printf("sum=%d\n",sum);

  /*create the thread 2*/
  // add your program 
  pthread_create(&tid2,&attr,B,argv[1]);  
  pthread_create(&tid3,&attr,C,argv[1]);
  
  /*send semaphore to thread 2 and begin the threads running*/
  sem_post(&one);
  /*wait for the thread to exit*/
  // add your program 
  pthread_join(tid1,NULL);
  pthread_join(tid2,NULL);  
  pthread_join(tid2,NULL);  
  printf("sum=%d\n",sum);

}

/*The thread will begin control in this function*/
void *A(void *param)
{
  //close(fd[0]);
  char str[100];
  file = fopen("data_file.txt", "r");
  

  while(fgets(str,1000, file)){
  //printf("%s",str);
  //printf("size of string: %i\n", strlen(str));
  if(strlen(str) > 0) 
  { 
  		write(fd[1], str, strlen(str)+1);
  		sem_post(&sread);
  		sem_wait(&swrite);  
  }
  }
  printf("Thread1 Flag"); 
  
  pthread_mutex_lock(&mutex);
 
  /* release the mutex lock */
  pthread_mutex_unlock(&mutex); 
  

  /* send semaphore to thread 2*/
  sem_post(&two);
  

}

/*The thread will begin control in this function*/
void *B(void *param)
{

  int n;
  
  while(1)
  {
   //close(fd[1]);
  		sem_wait(&sread);
 
  		n = read(fd[0], buf, MAX_BUF);
  		if(strlen(buf)>0)
  			{
  				printf("Recieved: %s\n", buf);
  			}
  //		printf("Size of N: %i\n",n);

  	//	printf("thread two\n");
	  /* release the mutex lock */
	  // add your program 
	 
	  sem_post(&swrite);   
	}
}

void *C(void *param)
{
	 
	   while(1)
	   {
	   //sem_wait(&swrite);
	   FILE* dataf = fopen("data.txt", "w");
 		fprintf(dataf,"balls");
 		}
 		
      fclose(dataf);

}

void initializeData() {
   sum=0;
   
   /* Create the mutex lock */
   pthread_mutex_init(&mutex, NULL);

   /* Create the one semaphore and initialize to 0 */
   sem_init(&one, 0, 0);

   /* Create the two semaphore and initialize to BUFFER_SIZE */
   sem_init(&two, 0, 0);

   /* Get the default attributes */
   pthread_attr_init(&attr);

   //Initialise Pipe

   if (pipe(fd) == -1)
   	printf("PIPE-ERROR");

}

