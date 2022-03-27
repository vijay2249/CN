#include<sys/socket.h> 
#include<netinet/in.h> //Internet addresses
#include<string.h> // memset
#include <sys/wait.h>

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 


#define PORT 1234
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
    addr.sin_addr.s_addr=INADDR_ANY;

    if(bind(sfd,(struct sockaddr *)&addr,sizeof(addr))==-1){
        perror("binding failed");
        exit(0);    
    }

    if(listen(sfd,1)==-1){
        perror("error in listen");
        exit(0);
    }
    
    int addrlen=sizeof(addr);
    int nsfd=accept(sfd,(struct sockaddr *)&addr, (socklen_t *)&addrlen);
    if(nsfd==-1){
        perror("error in accept");
        exit(0);
    }

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