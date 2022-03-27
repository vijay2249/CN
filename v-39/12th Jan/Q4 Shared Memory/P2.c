#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/sem.h>

#include<sys/types.h>
#include<sys/ipc.h>
#include <string.h>


int main()
{
     //Initalizing semaphore X with 0
    key_t keyS1 = ftok("newfileS1",65);
	int S1 = semget(keyS1,1,0666|IPC_CREAT);
	if(S1==-1)
	{
		perror("semget");
		exit(1);
	}
  semctl(S1,0,SETVAL,0);
	

     //Initalizing semaphore Y with 0
    key_t keyS2 = ftok("newfileS2",65);
	int S2 = semget(keyS2,1,0666|IPC_CREAT);
	if(S2==-1)
	{
		perror("semget");
		exit(1);
	}
  semctl(S2,0,SETVAL,0);
	

    //Shared memory X
    key_t keyX = ftok("shmfileX",65); 
    int shmidX = shmget(keyX,1024,0666|IPC_CREAT);

    //Shared memory Y
    key_t keyY = ftok("shmfileY",65); 
    int shmidY = shmget(keyY,1024,0666|IPC_CREAT);

    int *sX =  shmat(shmidX,(void*)0,0);
     *sX=1;
    int *sY = shmat(shmidY,(void*)0,0);
     *sY=1;
    printf("Initial value X :%d\n\n",*sX); 
     struct sembuf sb= { 0, 1, SEM_UNDO };
   while(1){
      printf("I am waiting for S1\n");
      //Waiting for S1
       sb.sem_op=-1;  
         if(semop(S1,&sb,1)==-1)
		        {
			          perror("semop");
			          exit(1);
		        }
        
         printf("I am reading shared memory X\n");  
        *sY=*sX+1;
        printf("X:%d \n",*sX);
        printf(" Enter any char to signal S2 :");

        char c;
		scanf(" %c",&c);

        sb.sem_op=1;  
         if(semop(S2,&sb,1)==-1)
		        {
		        	perror("semop");
		        	exit(1);
		        }
        
        printf("\n");
        
   }
    
    //detach from shared memory 
    shmdt(sX);
    shmdt(sY);
   

    // destroy the shared memory
    shmctl(shmidX,IPC_RMID,NULL);
    shmctl(shmidY,IPC_RMID,NULL); 
    return 0;
}