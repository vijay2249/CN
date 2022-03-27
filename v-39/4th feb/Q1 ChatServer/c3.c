#include<sys/socket.h> 
#include<netinet/in.h> //Internet addresses
#include<string.h> // memset
#include <sys/wait.h>
#include<arpa/inet.h>

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 
#include <sys/select.h>

void* recieve_msg(void * arg)
{
    int *ptr;
    int nsfd;
    ptr = (int *) arg;
    nsfd = *ptr;

    fd_set fdset;
    while(1){
     sleep(1);

           char buffer[100];
        memset(buffer,'\0',sizeof(buffer));
	    recv(nsfd,buffer,sizeof(buffer),0);
         if(buffer[0]!='\0') printf("Message recieved : %s",buffer);

	
    }
   
		
	
}

	
void* send_msg(void * arg)
{
    int *ptr;
    int nsfd;
    ptr = (int *) arg;
    nsfd = *ptr;
    
	char buffer[100];
	while(1){

        memset(buffer,'\0',sizeof(buffer));

        read(0,buffer,sizeof(buffer));
        if(send(nsfd,buffer,sizeof(buffer),0)==-1){
        perror("Sending failed");
        exit(0);
        }
        else{
            printf("Message sent\n");
        }

    }
		
	
}


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


    pthread_t thread_recieve,thread_send;
    
    pthread_create(&thread_recieve,NULL,recieve_msg,(void *)&sfd);
   
    pthread_create(&thread_send,NULL,send_msg,(void *)&sfd);

    void *retval;
    pthread_join(thread_recieve, &retval);
    pthread_join(thread_send, &retval);
   
	
}
	
