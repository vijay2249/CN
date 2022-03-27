#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<unistd.h>
struct mymesg{
	long mtype;
	char mtext[1024];
};
int main()
{
	sleep(3);
	key_t key = ftok("file",65);
	int msqid = msgget(key,0666| IPC_CREAT);
	printf("%d\n",msqid);
	
	struct mymesg ptr1;
	int siz = msgrcv(msqid,&ptr1,sizeof(ptr1),1,IPC_NOWAIT);
	if(siz==-1)
	{
		printf("\nmsg not received\n");
		return 0;
	}
	printf("\nMessage received by p1 from p2 is : %s \n",ptr1.mtext);
	
	struct mymesg ptr2;
	siz = msgrcv(msqid,&ptr2,sizeof(ptr2),2,IPC_NOWAIT);
	if(siz==-1)
	{
		printf("\nmsg not received\n");
		return 0;
	}
	printf("\nMessage received by p1 from p3 is : %s \n",ptr2.mtext);
	
	struct mymesg ptr3;
	siz = msgrcv(msqid,&ptr3,sizeof(ptr3),3,IPC_NOWAIT);
	if(siz==-1)
	{
		printf("\nmsg not received\n");
		return 0;
	}
	printf("\nMessage received by p1 from p4 is : %s \n",ptr3.mtext);
	
	struct mymesg m1;
	strcpy(m1.mtext,"message fron P1 to P2");
	//printf("done1\n");
	m1.mtype=atol(ptr1.mtext);
	//printf("%ld\n",m1.mtype);
	//printf("done2\n");
	int c= msgsnd(msqid,&m1,sizeof(m1),IPC_NOWAIT);
	if(c==-1)
	{
		printf("\nmsg not sent\n");
		return 0;
	}
	//printf("done3\n");
	
	struct mymesg m2;
	strcpy(m2.mtext,"message from P1 to P3");
	//printf("done4\n");
	m2.mtype=atol(ptr2.mtext);
	c= msgsnd(msqid,&m2,sizeof(m2),IPC_NOWAIT);
	if(c==-1)
	{
		printf("\nmsg not sent\n");
		return 0;
	}
	
	struct mymesg m3;
	strcpy(m3.mtext,"message from P1 to P4");
	m3.mtype=atol(ptr3.mtext);
	c= msgsnd(msqid,&m3,sizeof(m3),IPC_NOWAIT);
	if(c==-1)
	{
		printf("\nmsg not sent\n");
		return 0;
	}
	return 0;
}