#include<sys/socket.h> 
#include<netinet/in.h> //Internet addresses
#include<string.h> // memset
#include <sys/wait.h>
#include<arpa/inet.h>

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 

#define PORT 6060
void* server_thread(void * arg)
{
    int *ptr;
    int nsfd;
    ptr = (int *) arg;
    nsfd = *ptr;

	 char buffer[100];
		 memset(buffer,'\0',sizeof(buffer));
         read(0,buffer,sizeof(buffer));
        //Sending message to Mediator
		int s = send(nsfd,buffer,sizeof(buffer),0);
        
		if(s==-1)
		{
			write(1,"send error\n",sizeof("send error\n"));
			exit(0);
		}

        memset(buffer,'\0',sizeof(buffer));
        recv(nsfd,buffer,sizeof(buffer),0);
        
		printf("Message from Client :%s\n",buffer);
		
	
}

int main(){
    
    
    int sfd;
    sfd=socket(AF_INET,SOCK_STREAM,0);
    if(sfd==-1){
        perror("Socket creation failed");
        exit(0);
    }

    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(PORT);
    addr.sin_addr.s_addr=inet_addr("127.0.0.1");

    if(bind(sfd,(struct sockaddr *)&addr,sizeof(addr))==-1){
        perror("binding failed");
        exit(0);    
    }
    if(listen(sfd,1)==-1){
        perror("error in listen");
        exit(0);
    }
   
    while(1){
    int addrlen1=sizeof(addr);
    int nsfd=accept(sfd,(struct sockaddr *)&addr, (socklen_t *)&addrlen1);
    if(nsfd==-1){
        perror("error in accept");
        exit(0);
    }
     pthread_t thread1;
     pthread_create(&thread1,NULL,server_thread,(void *)&nsfd);

        pthread_join(thread1, NULL);
        printf("\n------------------------------------------------------------\n");
    }
			
}