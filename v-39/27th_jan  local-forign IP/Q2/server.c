#include<sys/socket.h> 
#include<netinet/in.h> //Internet addresses
#include<string.h> // memset
#include <sys/wait.h>
#include<arpa/inet.h>

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 

#include<poll.h>


#include<netdb.h>
#include<sys/types.h>
#include<ctype.h>

void* TS1(void * arg)
{
    int *ptr;
    int nsfd;
    ptr = (int *) arg;
    nsfd = *ptr;

	char buffer[100];
	recv(nsfd,buffer,sizeof(buffer),0);
	printf("Msg from client1: %s\n",buffer);
		
	
}
void* TS2(void * arg)
{
	 int *ptr;
    int nsfd;
    ptr = (int *) arg;
    nsfd = *ptr;
	
		char buffer[100];
		recv(nsfd,buffer,sizeof(buffer),0);
		printf("Msg from client2: %s\n",buffer);

		for(int i=0;i<sizeof(buffer);i++)
		{
			buffer[i]=toupper(buffer[i]);
		}
		send(nsfd,buffer,sizeof(buffer),0);

	
}
void* TS3(void * arg)
{
	 int *ptr;
    int nsfd;
    ptr = (int *) arg;
    nsfd = *ptr;
	
		char buffer[100];
		recv(nsfd,buffer,sizeof(buffer),0);
		printf("Msg from client3: %s\n",buffer);

		for(int i=0;i<sizeof(buffer);i++)
		{
			buffer[i]=tolower(buffer[i]);
		}
		send(nsfd,buffer,sizeof(buffer),0);
	
	
}

int main(){
    
    // Socket 1 creation
    int sfd1;
    sfd1=socket(AF_INET,SOCK_STREAM,0);
    if(sfd1==-1){
        perror("Socket creation failed");
        exit(0);
    }

    struct sockaddr_in addr1;
    addr1.sin_family=AF_INET;
    addr1.sin_port=htons(1111);
    addr1.sin_addr.s_addr=INADDR_ANY;

    if(bind(sfd1,(struct sockaddr *)&addr1,sizeof(addr1))==-1){
        perror("binding failed");
        exit(0);    
    }
    if(listen(sfd1,1)==-1){
        perror("error in listen");
        exit(0);
    }
    else{
        printf("socket 1 listening \t");
    }
    int addrlen1=sizeof(addr1);
    int nsfd1=accept(sfd1,(struct sockaddr *)&addr1, (socklen_t *)&addrlen1);
    if(nsfd1==-1){
        perror("error in accept");
        exit(0);
    }
     else{
        printf("Socket 1 accepted request\n");
    }
     
    
    // Socket 2 creation
    int sfd2;
    sfd2=socket(AF_INET,SOCK_STREAM,0);
    if(sfd2==-1){
        perror("Socket creation failed");
        exit(0);
    }

    struct sockaddr_in addr2;
    addr2.sin_family=AF_INET;
    addr2.sin_port=htons(2222);
    addr2.sin_addr.s_addr=INADDR_ANY;

    if(bind(sfd2,(struct sockaddr *)&addr2,sizeof(addr2))==-1){
        perror("binding failed");
        exit(0);    
    }
    if(listen(sfd2,2)==-1){
        perror("error in listen");
        exit(0);
    }
    else{
        printf("socket 2 listening \t");
    }
     int addrlen2=sizeof(addr2);
    int nsfd2=accept(sfd2,(struct sockaddr *)&addr2, (socklen_t *)&addrlen2);
    if(nsfd2==-1){
        perror("error in accept");
        exit(0);
    }
     else{
        printf("Socket 2 accepted request\n");
    }
     
    

    // Socket 3 creation
    int sfd3;
    sfd3=socket(AF_INET,SOCK_STREAM,0);
    if(sfd3==-1){
        perror("Socket creation failed");
        exit(0);
    }

    struct sockaddr_in addr3;
    addr3.sin_family=AF_INET;
    addr3.sin_port=htons(3333);
    addr3.sin_addr.s_addr=INADDR_ANY;

    if(bind(sfd3,(struct sockaddr *)&addr3,sizeof(addr3))==-1){
        perror("binding failed");
        exit(0);    
    }
    if(listen(sfd3,3)==-1){
        perror("error in listen");
        exit(0);
    }
    else{
        printf("socket 3 listening \t");
    }
    int addrlen3=sizeof(addr3);
    int nsfd3=accept(sfd3,(struct sockaddr *)&addr3, (socklen_t *)&addrlen3);
    if(nsfd3==-1){
        perror("error in accept");
        exit(0);
    }
    else{
        printf("Socket 3 accepted request\n");
    }
    
    struct pollfd pfds[3];
     while(1){
        
	    pfds[0].fd=nsfd1;
	    pfds[0].events=POLLIN;
	    pfds[0].revents=0;

	    pfds[1].fd=nsfd2;
	    pfds[1].events=POLLIN;
	    pfds[1].revents=0;

	    pfds[2].fd=nsfd3;
	    pfds[2].events=POLLIN;
	    pfds[2].revents=0;
        
        poll(pfds,3,500);
        int count = poll(pfds,3,500);
		if(count==-1)
		perror("poll error");
		else{
			pthread_t thread[3];

			if(pfds[0].revents && POLLIN)
			{	
				pthread_create(&thread[0],NULL,TS1,(void *)&nsfd1);
			}
            if(pfds[1].revents && POLLIN)
			{	
				pthread_create(&thread[1],NULL,TS2,(void *)&nsfd2);   
			}
            if(pfds[2].revents && POLLIN)
			{	
				pthread_create(&thread[2],NULL,TS3,(void *)&nsfd3);
			}

          }
      
      }
     

      close(sfd1);
      close(nsfd1);
      close(sfd2);
      close(nsfd2);
      close(sfd3);
      close(nsfd3);
}