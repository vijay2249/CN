#include<sys/socket.h>
#include<netinet/in.h> //Internet addresses
#include<string.h> // memset
#include<arpa/inet.h>//inet_addr

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 

#define PORT 1234
int main(){
    int sfd;
    //creating a socket
    sfd=socket(AF_INET,SOCK_STREAM,0);
    if(sfd==-1){
        perror("Socket creation failed");
        exit(0);
    }
    struct sockaddr_in addr;

    addr.sin_family=AF_INET;
    addr.sin_port=htons(PORT);
    //addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    if(inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr)==-1) 
	{ 
		perror("Invalid Address");
		exit(0); 
	} 

    //connecting
    if(connect(sfd,(struct sockaddr *) &addr,sizeof(addr))==-1){
        perror("Connection Failed !");
        exit(0);
    }
    
    //getting local IP details
     struct sockaddr_in localaddr;
     int loc_len=sizeof(localaddr);
     getsockname(sfd,(struct sockaddr * restrict)&localaddr,&loc_len);

    printf("Local IP address is: %s\n", inet_ntoa(localaddr.sin_addr));
    printf("Local port is: %d\n", (int)ntohs(localaddr.sin_port));
    
    //getting foreign IP details
    struct sockaddr_in foreignaddr;
     int foreign_len=sizeof(foreignaddr);
     getpeername(sfd,(struct sockaddr * restrict)&foreignaddr,&foreign_len);
     printf("\nForeign IP address is: %s\n", inet_ntoa(foreignaddr.sin_addr));
     printf("Foreign port is: %d\n\n", (int)ntohs(foreignaddr.sin_port));



   char buffer[1024]; 
   memset(buffer,'\0',sizeof(buffer));
   //printf("Enter msg to be send to server :");
   	read(0,buffer,sizeof(buffer));
   if(send(sfd,buffer,sizeof(buffer),0)==-1){
       perror("Sending failed");
       exit(0);
   }

    memset(buffer,'\0',sizeof(buffer));
    if(recv(sfd,buffer,sizeof(buffer),0)==-1){
        perror("Message not recieved");
        exit(0);
    } 
	printf("message recieved by client:%s",buffer);

     close(sfd);

}