#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

#include<signal.h>

#include<fcntl.h>

pid_t left,right;
struct mymesg{
	long mtype;
	char mtext[20];
};
void handler1(int signo, siginfo_t *info,void * context)
{
	left = (int)info->si_pid;

	//printf("%d:P1 pid \n",left);
	return;

}
void handel(int sig){
    printf("P1 signaling P2\n");
}
void handel2(int sig){
    printf("P3 signaling P2\n");
}
int main()
{
  
	key_t key = ftok("newfile",65);
	int msqid = msgget(key,0666| IPC_CREAT);
	int pid=(int)getpid();
    printf("P2 pid from P2 :%d\n",pid);
	struct mymesg ptr;
	ptr.mtype=2;
	sprintf(ptr.mtext,"%d",pid);
	int c= msgsnd(msqid,&ptr,sizeof(ptr),IPC_NOWAIT);
	if(c==-1)
	{
		printf("\nmsg not sent\n");
		return 0;
	}
	
	
    //Get P1 pid using sigaction()
	
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
    printf("P1 pid inside P2:%d\n",left);

   

    //get P3 msg from msgqueue
    struct mymesg p3msg;
	int siz = msgrcv(msqid,&p3msg,sizeof(p3msg),3,IPC_NOWAIT);
	if(siz==-1)
	{
		printf("\nmsg not received\n");
		return 0;
	}
    
	//printf("\nMessage received by p2 from p3 is : %s \n",p3msg.mtext);
	
	right=atoi(p3msg.mtext);  
    printf("P3 pid from P2 : %d\n",right);
    
   //P2 sends msqqueue of P1 to P4
    struct mymesg ptr2;
	ptr2.mtype=1;
	sprintf(ptr2.mtext,"%d",left);
	int c1= msgsnd(msqid,&ptr2,sizeof(ptr2),IPC_NOWAIT);
	if(c1==-1)
	{
		printf("\nmsg not sent\n");
		return 0;
	}
    
	//P2 sends SIGUSR1 to P3
	 kill(right,SIGUSR1);
     
	 printf("\n");
    //Circular signaling
	signal(SIGUSR1,handel);
	signal(SIGUSR2,handel2);

	sleep(12);
	 kill(right,SIGUSR1);
    sleep(12);
	 kill(right,SIGUSR1);
	sleep(12);
	 kill(right,SIGUSR1);
    
	printf("\n");
    
	sleep(16);
	kill(left,SIGUSR2);  
	sleep(24);
	kill(left,SIGUSR2);  
	sleep(30);
	kill(left,SIGUSR2);  
    close(msqid);

    close(left);
    close(right);
}