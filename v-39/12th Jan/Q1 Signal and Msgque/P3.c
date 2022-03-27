#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

#include<signal.h>

  pid_t left,right;

struct mymesg{
	long mtype;
	char mtext[20];
};
void handler1(int signo, siginfo_t *info,void * context)
{
	left = (int)info->si_pid;

	//printf("%d:P2 pid \n",left);
	return;

}
void handel(int sig){
    printf("P2 signaling P3\n");
}
void handel2(int sig){
    printf("P4 signaling P3\n");
}

int main()
{
	
  
	key_t key = ftok("newfile",65);
	int msqid = msgget(key,0666| IPC_CREAT);
	int pid=(int)getpid();
    printf("P3 pid from P3 :%d\n",pid);
	struct mymesg ptr;
	ptr.mtype=3;
	sprintf(ptr.mtext,"%d",pid);
	int c= msgsnd(msqid,&ptr,sizeof(ptr),IPC_NOWAIT);
	if(c==-1)
	{
		printf("\nmsg not sent\n");
		return 0;
	}

	//sleep(8);
   
    //P3  catch the signal sent by P2 
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
    printf("P2 pid inside P3:%d\n",left);
   
   
    //P3 recieves msgqueue from P4
      struct mymesg p4msg;
	int siz = msgrcv(msqid,&p4msg,sizeof(p4msg),4,IPC_NOWAIT);
	if(siz==-1)
	{
		printf("\nmsg not received\n");
		return 0;
	}
    
	//printf("\nMessage received by p3 from p4 is : %s \n",p4msg.mtext);
	right=atoi(p4msg.mtext);  
    printf("P4 pid from P3 %d\n",right);
    
	//P3 send SIGUSR1 to P4
	kill(right,SIGUSR1);
    
	printf("\n");

	  //Circular signaling
	  signal(SIGUSR1,handel);
	  signal(SIGUSR2,handel2);

	  sleep(16);
	 kill(right,SIGUSR1);
	  sleep(16);
	 kill(right,SIGUSR1);
	   sleep(16);
	 kill(right,SIGUSR1);

	 printf("\n");
     

	 sleep(12);
	kill(left,SIGUSR2);   
	sleep(24);
	kill(left,SIGUSR2);  
	sleep(28);
	kill(left,SIGUSR2);  

	  close(msqid);

      close(left);
      close(right);
	

}