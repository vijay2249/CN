#include<sys/socket.h> 
#include<netinet/in.h> //Internet addresses
#include<string.h> // memset
#include<sys/wait.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h> 
#include<unistd.h> 

#define PORT 1234

int main(){
    
    int sfd;
    //creating socket 
    sfd=socket(AF_INET,SOCK_STREAM,0);
    if(sfd==-1){
        perror("Socket creation failed");
        exit(0);
    }
    struct sockaddr_in addr;

    addr.sin_family=AF_INET;
    addr.sin_port=htons(PORT);
    addr.sin_addr.s_addr=INADDR_ANY;
    //binding
    if(bind(sfd,(struct sockaddr *)&addr,sizeof(addr))==-1){
        perror("binding failed");
        exit(0);
    }
     struct sockaddr_in localaddr;
     int loc_len=sizeof(localaddr);
     getsockname(sfd,(struct sockaddr * restrict)&localaddr,&loc_len);

    sleep(5);
    printf("Local IP address is: %s\n", inet_ntoa(localaddr.sin_addr));
    printf("Local port is: %d\n", (int)ntohs(localaddr.sin_port));

    //listening
    if(listen(sfd,1)==-1){
        perror("error in listen");
        exit(0);
    }

    //accept
    int addrlen=sizeof(addr);
    int nsfd=accept(sfd,(struct sockaddr *)&addr, (socklen_t *)&addrlen);
    if(nsfd==-1){
        perror("error in accept");
        exit(0);
    }

     struct sockaddr_in foreignaddr;
     int foreign_len=sizeof(foreignaddr);
     getpeername(sfd,(struct sockaddr * restrict)&foreignaddr,&foreign_len);
     printf("\nForeign IP address is: %s\n", inet_ntoa(foreignaddr.sin_addr));
     printf("Foreign port is: %d\n\n", (int)ntohs(foreignaddr.sin_port));


    int c=fork();
    if(c>0){
         close(nsfd);
         wait(NULL);
         close(sfd);
    }
    else{
        sleep(5);
        close(sfd);
        char buffer[1024];
        memset(buffer,'\0',sizeof(buffer));
		int valread = recv( nsfd , buffer, 1024,0);
		printf("message recieved by server: %s",buffer);

        memset(buffer,'\0',sizeof(buffer));
		//printf("Enter msg:");
		read(0,buffer,sizeof(buffer));

		send(nsfd,buffer,sizeof(buffer),0); 
        close(nsfd);
    }
}
