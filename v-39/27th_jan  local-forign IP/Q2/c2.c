#include<sys/socket.h> 
#include<netinet/in.h> //Internet addresses
#include<string.h> // memset
#include <sys/wait.h>
#include<arpa/inet.h>

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 
int main()
{
	int sfd;
	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd==-1)
	{
		printf("socket creation error\n");
		exit(0);
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(2222);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	

	if(connect(sfd,(struct sockaddr*) &addr,sizeof(addr))!=0)
	{
		printf("Connect failed!\n");
		exit(0);
	}

	
	while(1)
	{
		char buffer[200];
		memset(buffer,0,sizeof(buffer));
		printf("Enter msg to server from client: ");
		scanf("%s",buffer);
		int s = send(sfd,buffer,sizeof(buffer),0);
		if(s==-1)
		{
			write(1,"send error\n",sizeof("send error\n"));
			exit(0);
		}
		else
			write(1,"message sent\n",sizeof("message sent\n"));
			
		if(strcmp(buffer,"done")==0)
			return 0;

        sleep(2);	
		char buffer2[200];
		memset(buffer2,0,sizeof(buffer2));
		int r = recv(sfd,buffer2,sizeof(buffer2),0);
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
			printf("Msg from server: %s\n",buffer2);
			
		
	}
	
	close(sfd);
}
	
