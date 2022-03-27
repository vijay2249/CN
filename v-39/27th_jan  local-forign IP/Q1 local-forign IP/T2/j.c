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
	servaddr.sin_port = htons(8080);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	//bind
	if(bind(sfd,(struct sockaddr *)&servaddr,sizeof(servaddr))==-1)
	{
		printf("binding failed\n");
		exit(0);
	}
	else
		printf("Socket binding successful\n");
		
		
	//getsockname
	struct sockaddr_in laddr;
	socklen_t len = sizeof(laddr);
	getsockname(sfd,(struct sockaddr *)&laddr,&len);
	char lIP[16];
	int lport;
	inet_ntop(AF_INET, &laddr.sin_addr, lIP, sizeof(lIP));
    	lport = ntohs(laddr.sin_port);
	printf("Local ip address: %s\n",lIP);
	printf("Local port : %d\n",lport);
		
	//listen
	if(listen(sfd,3)!=0)
	{
		printf("socket listen failed\n");
		exit(0);
	}
	else
		printf("socket listening!\n");
		
	while(1)
	{
	//accept
	struct sockaddr_in client;
	int len = sizeof(client);
	int nsfd = accept(sfd,(struct sockaddr *restrict)&client,&len);
	if(nsfd==-1)
	{
		printf("accept() failed!\n");
		exit(0);
	}
	else
		printf("\nAccept() success!\n");
		
		
	//getpeername
	struct sockaddr_in faddr;
	socklen_t len1 = sizeof(faddr);
	getpeername(nsfd,(struct sockaddr *)&faddr,&len1);
	char fIP[16];
	int fport;
	inet_ntop(AF_INET, &faddr.sin_addr, fIP, sizeof(fIP));
    	fport = ntohs(faddr.sin_port);
	printf("Foreign ip address: %s\n",fIP);
	printf("Foreign port : %d\n",fport);
		
	int c;
	c = fork();
	if(c>0)
	{
		//parent
		close(nsfd);
	}
	else
	{
		//child
		close(sfd);
		
		while(1)
		{
			char rbuf[200];
			recv(nsfd,rbuf,sizeof(rbuf),0);
			printf("Msg from client: %s\n",rbuf);
			if(strcmp(rbuf,"done")==0)
				return 0;
				
			char sbuf[200];
			printf("Enter msg to client from server: ");
			scanf("%s",sbuf);
			send(nsfd,sbuf,sizeof(sbuf),0);
		}
	}
	}
	
}