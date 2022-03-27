#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<poll.h>
#include<pthread.h>
#include<ctype.h>
void* TS0(void * arg)
{
	int nsfd = (int)arg;
	while(1)
	{
		char rbuf[200];
		recv(nsfd,rbuf,sizeof(rbuf),0);
		printf("Msg from client1: %s\n",rbuf);
		if(strcmp(rbuf,"done")==0)
			pthread_exit(NULL);
	}
}
void* TS1(void * arg)
{
	int nsfd = (int)arg;
	while(1)
	{
		char rbuf[200];
		recv(nsfd,rbuf,sizeof(rbuf),0);
		//printf("Msg from client1: %s\n",rbuf);
		//capitalise
		for(int i=0;i<sizeof(rbuf);i++)
		{
			rbuf[i]=toupper(rbuf[i]);
		}
		send(nsfd,rbuf,sizeof(rbuf),0);
		if(strcmp(rbuf,"DONE")==0)
			pthread_exit(NULL);
	}
}
void* TS2(void * arg)
{
	int nsfd = (int)arg;
	while(1)
	{
		char rbuf[200];
		recv(nsfd,rbuf,sizeof(rbuf),0);
		//printf("Msg from client1: %s\n",rbuf);
		//lower letter
		for(int i=0;i<sizeof(rbuf);i++)
		{
			rbuf[i]=tolower(rbuf[i]);
		}
		send(nsfd,rbuf,sizeof(rbuf),0);
		if(strcmp(rbuf,"done")==0)
			pthread_exit(NULL);
	}
}
int main()
{
	int sfd1,sfd2,sfd3;
	//socket
	sfd1 = socket(AF_INET,SOCK_STREAM,0);
	if(sfd1==-1)
	{
		printf("socket creation error\n");
		exit(0);
	}
	struct sockaddr_in servaddr1;
	servaddr1.sin_family = AF_INET;
	servaddr1.sin_port = htons(8080);
	servaddr1.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	//bind
	if(bind(sfd1,(struct sockaddr *)&servaddr1,sizeof(servaddr1))==-1)
	{
		printf("binding1 failed\n");
		exit(0);
	}
	else
		printf("Socket1 binding successful\n");
		
	//listen
	if(listen(sfd1,2)!=0)
	{
		printf("socket1 listen failed\n");
		exit(0);
	}
	else
		printf("socket1 listening!\n");
	
	
	//sfd2
	sfd2 = socket(AF_INET,SOCK_STREAM,0);
	if(sfd1==-1)
	{
		printf("socket creation error\n");
		exit(0);
	}
	struct sockaddr_in servaddr2;
	servaddr2.sin_family = AF_INET;
	servaddr2.sin_port = htons(4444);
	servaddr2.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	//bind
	if(bind(sfd2,(struct sockaddr *)&servaddr2,sizeof(servaddr2))==-1)
	{
		printf("binding2 failed\n");
		exit(0);
	}
	else
		printf("Socket2 binding successful\n");
	
	//listen
	if(listen(sfd2,2)!=0)
	{
		printf("socket2 listen failed\n");
		exit(0);
	}
	else
		printf("socket2 listening!\n");
	
	//sfd3
	sfd3 = socket(AF_INET,SOCK_STREAM,0);
	if(sfd1==-1)
	{
		printf("socket creation error\n");
		exit(0);
	}
	struct sockaddr_in servaddr3;
	servaddr3.sin_family = AF_INET;
	servaddr3.sin_port = htons(5678);
	servaddr3.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	//bind
	if(bind(sfd3,(struct sockaddr *)&servaddr3,sizeof(servaddr3))==-1)
	{
		printf("binding3 failed\n");
		exit(0);
	}
	else
		printf("Socket3 binding successful\n");
		
	//listen
	if(listen(sfd3,2)!=0)
	{
		printf("socket3 listen failed\n");
		exit(0);
	}
	else
		printf("socket3 listening!\n");
		
		
		
	//POLL
	struct pollfd fdarray[3];
	fdarray[0].fd=sfd1;
	fdarray[0].events = POLLIN;
	
	fdarray[1].fd=sfd2;
	fdarray[1].events = POLLIN | POLLOUT;
	
	fdarray[2].fd=sfd3;
	fdarray[2].events = POLLIN | POLLOUT;
	
	poll(fdarray,3,2);
	
		
		
	//threads
	pthread_t ts[3];
	while(1)
	{
	struct sockaddr_in client1;
	int len1 = sizeof(client1);
	int nsfd1 = accept(sfd1,(struct sockaddr *restrict)&client1,&len1);
	if(nsfd1==-1)
	{
		printf("accept() failed!\n");
		exit(0);
	}
	else
		printf("\nAccept1 success!\n");
		
	pthread_create(&ts[0],NULL,&TS0,(void *)nsfd1);
	
		
	struct sockaddr_in client2;
	int len2 = sizeof(client2);
	int nsfd2 = accept(sfd2,(struct sockaddr *restrict)&client2,&len2);
	if(nsfd2==-1)
	{
		printf("accept() failed!\n");
		exit(0);
	}
	else
		printf("\nAccept2 success!\n");
		
	pthread_create(&ts[1],NULL,&TS1,(void *)nsfd2);
	
	struct sockaddr_in client3;
	int len3 = sizeof(client3);
	int nsfd3 = accept(sfd3,(struct sockaddr *restrict)&client3,&len3);
	if(nsfd3==-1)
	{
		printf("accept() failed!\n");
		exit(0);
	}
	else
		printf("\nAccept3 success!\n");
		
	pthread_create(&ts[2],NULL,&TS2,(void *)nsfd3);
	}
			
}