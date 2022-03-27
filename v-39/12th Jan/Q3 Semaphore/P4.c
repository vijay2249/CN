#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#include<sys/types.h>
#include<sys/ipc.h>

#include<sys/sem.h>


int main()
{
	//creating semaphores
	key_t key34 = ftok("newfile34",65);
	int s34 = semget(key34,1,0666|IPC_CREAT);
	if(s34==-1)
	{
		perror("semget");
		exit(1);
	}
	
	key_t key41 = ftok("newfile41",65);
	int s41 = semget(key41,1,0666|IPC_CREAT);
	if(s41==-1)
	{
		perror("semget");
		exit(1);
	}
	
	//Initializing to 0
	semctl(s34,0,SETVAL,0);
	semctl(s41,0,SETVAL,0);
	
	while(1)
	{
		printf("I am P4. I am waiting for Semaphore S34 \n");
        struct sembuf sb= { 0, -1, SEM_UNDO };
		if(semop(s34,&sb,1)==-1)
		{
			perror("semop");
			exit(1);
		}
 
        printf("I got semaphore S34 signalling from P1\n");

        printf("Enter any character to sem-signal( S41)\n");
		char c;
		scanf(" %c",&c);
		
        printf("I am signalling semaphore signal of S41\n");
		sb.sem_op=1;
		if(semop(s41,&sb,1)==-1)
		{
			perror("semop");
			exit(1);
		}
	    
        printf("\n---------------------------------------------------------------------------\n");
		sleep(1);
	}
		
	return 0;
}