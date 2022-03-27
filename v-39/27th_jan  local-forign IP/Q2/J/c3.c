#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include<unistd.h>
#include<arpa/inet.h>
int main()
{
	int sfd;
	//socket
	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd==-1)
	{
		printf("socket creation error\n");
		exit(0);
	}
	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(5678);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	//client
	if(connect(sfd,(struct sockaddr*) &servaddr,sizeof(servaddr))!=0)
	{
		printf("Connect failed!\n");
		exit(0);
	}
	else
	{
		printf("Client connected to server!\n");
	}
	
	
	while(1)
	{
		char sbuf[200];
		memset(sbuf,0,sizeof(sbuf));
		printf("Enter msg to server from client: ");
		scanf("%s",sbuf);
		int s = send(sfd,sbuf,sizeof(sbuf),0);
		if(s==-1)
		{
			write(1,"send error\n",sizeof("send error\n"));
			exit(0);
		}
		else
			write(1,"message sent\n",sizeof("message sent\n"));
			
		if(strcmp(sbuf,"DONE")==0)
			return 0;
			
		char rbuf[200];
		memset(rbuf,0,sizeof(rbuf));
		int r = recv(sfd,rbuf,sizeof(rbuf),0);
		if(r==-1)
		{
			write(1,"recv error\n",sizeof("recv error\n"));
			exit(0);
		}
		else if(r==0)
		{
			write(1,"no messages available\n",sizeof("no messages available\n"));
		}
		else
			printf("Msg from server: %s\n",rbuf);
			
		
	}
	
	close(sfd);
}
	
