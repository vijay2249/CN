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
	addr.sin_port = htons(8080);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	if(connect(sfd,(struct sockaddr*) &addr,sizeof(addr))!=0)
	{
		printf("Connect failed!\n");
		exit(0);
	}
	

    char name[2];
    write(1,"Enter server number needed to connect :",40);
    read(0,name,sizeof(name));

       
        int s = send(sfd,name,sizeof(name),0);
		if(s==-1)
		{
			write(1,"Server number sending error\n",sizeof("send error\n"));
			exit(0);
		}

        
        char buffer[1024]; 
        memset(buffer,'\0',sizeof(buffer));
		recv(sfd,buffer,sizeof(buffer),0);
       
		printf("Message from server  :%s\n",buffer);

        memset(buffer,'\0',sizeof(buffer));
        read(0,buffer,sizeof(buffer));
        s = send(sfd,buffer,sizeof(buffer),0);
        
	
	close(sfd);
}
	
