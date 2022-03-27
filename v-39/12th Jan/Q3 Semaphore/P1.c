#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#include<sys/types.h>
#include<sys/ipc.h>

#include<sys/sem.h>

int main()
{   
	//creating semaphores
	key_t key12 = ftok("newfile12",65);
	int s12 = semget(key12,1,0666|IPC_CREAT);
	if(s12==-1)
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
	semctl(s12,0,SETVAL,0);
	semctl(s41,0,SETVAL,0);
	
	
	while(1)
	{
		printf("I am P1. Enter any character to sem-signal( S12).\n");
		char c;
		scanf(" %c",&c);
		printf("I am signalling semaphore signal of S12.\n");
		//unlock
		struct sembuf sb= { 0, 1, SEM_UNDO };
		if(semop(s12,&sb,1)==-1)
		{
			perror("semop");
			exit(1);
		}
		printf("I am waiting for semaphore S41.\n");
		
		sb.sem_op=-1;
		if(semop(s41,&sb,1)==-1)
		{
			perror("semop");
			exit(1);
		}
		printf("I got semaphore signalling from P4.\n ");
		printf("\n---------------------------------------------------------------------------\n");
		sleep(1);
	}
		
	return 0;
}