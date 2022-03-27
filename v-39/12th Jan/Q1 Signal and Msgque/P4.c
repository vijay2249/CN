#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

#include<signal.h>
struct mymesg{
	long mtype;
	char mtext[20];
};
pid_t left,right;
void handler1(int signo, siginfo_t *info,void * context)
{
	left = (int)info->si_pid;

	//printf("%d:P3 pid \n",left);
	return;

}
void handel(int sig){
    printf("P3 signaling P4\n");
}
void handel2(int sig){
    printf("P1 signaling P4\n");
}

int main()
{
     
	key_t key = ftok("newfile",65);
	int msqid = msgget(key,0666| IPC_CREAT);
	int pid=(int)getpid();
    printf("P4 pid from P4 :%d\n",pid);
	struct mymesg ptr;
	ptr.mtype=4;
	sprintf(ptr.mtext,"%d",pid);
	int c= msgsnd(msqid,&ptr,sizeof(ptr),IPC_NOWAIT);
	if(c==-1)
	{
		printf("\nmsg not sent\n");
		return 0;
	}

    //Catch P3 Signal
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
    printf("P3 pid inside P4:%d\n",left);


    //P4 catches P2 msgqueue msg 
    struct mymesg p2msg;
	int siz = msgrcv(msqid,&p2msg,sizeof(p2msg),1,IPC_NOWAIT);
	if(siz==-1)
	{
		printf("\nmsg not received\n");
		return 0;
	}
    
	//printf("\nMessage received by p4 from p2 of p1 pid is : %s \n",p2msg.mtext);
	right=atoi(p2msg.mtext);  
    printf("P1 pid from P4 %d\n",right);

	//Send USR1 Signal to P1
	kill(right,SIGUSR1);

    printf("\n");

	  //Circular signaling
	signal(SIGUSR1,handel);
	signal(SIGUSR2,handel2);

	sleep(20);
	 kill(right,SIGUSR1);
	 sleep(20);
	 kill(right,SIGUSR1);
	  sleep(20);
	 kill(right,SIGUSR1);

	 printf("\n");
    
	sleep(8);
	kill(left,SIGUSR2);   
	sleep(20);
	kill(left,SIGUSR2);  
	sleep(27);
	kill(left,SIGUSR2);  
	  
	  close(msqid);

      close(left);
      close(right);
}