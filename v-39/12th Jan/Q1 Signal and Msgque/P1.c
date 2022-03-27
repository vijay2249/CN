#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

#include<unistd.h>

#include<signal.h>
pid_t left,right;
struct mymesg{
	long mtype;
	char mtext[20];
};

void handler1(int signo, siginfo_t *info,void * context)
{
	left = (int)info->si_pid;

	//printf("%d:P3 pid \n",left);
	return;

}
void handel(int sig){
    printf("P4 signaling P1\n");
}

void handel2(int sig){
    printf("P2 signaling P1\n");
}

int main()
{
	sleep(3);

	key_t key = ftok("newfile",65);
	int msqid = msgget(key,0666| IPC_CREAT);
	// printf("%d\n",msqid);
	
	struct mymesg ptr1;
	int siz = msgrcv(msqid,&ptr1,sizeof(ptr1),2,IPC_NOWAIT);
	if(siz==-1)
	{
		printf("\nmsg not received\n");
		return 0;
	}
     
	//printf("\nMessage received by p1 from p2 is : %s \n",ptr1.mtext);
	right=atoi(ptr1.mtext);
	printf("P2 pid inside P1:%d\n",right);
     //signal(SIGUSR1,handel);
     
	
     kill(right,SIGUSR1);
      printf("P1 pid from P1:%d\n",getpid());

    //Recieve P4 signal
         struct sigaction sig;
    sig.sa_flags = SA_SIGINFO;
	sig.sa_sigaction=handler1;
	sigemptyset(&sig.sa_mask);
	 
	if(sigaction(SIGUSR1, &sig, NULL)==-1)
	{
		printf("error\n");
		return 0;
	}
   sleep(8);
    printf("P4 pid inside P1:%d\n",left);
    
	printf("\n");
    //Circluar signaling
	signal(SIGUSR1,handel);
	signal(SIGUSR2,handel2);

	sleep(8);
    kill(right,SIGUSR1);
	sleep(20);
	kill(right,SIGUSR1);
	sleep(20);
    kill(right,SIGUSR1);
	sleep(20);
    
	printf("\n");

    kill(left,SIGUSR2); 
	sleep(20);
	kill(left,SIGUSR2); 
	sleep(20);
	kill(left,SIGUSR2); 
	sleep(30);
      close(msqid);
      close(left);
    close(right);
}
