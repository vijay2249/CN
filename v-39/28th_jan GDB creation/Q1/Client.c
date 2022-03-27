#include<sys/socket.h>
#include<netinet/in.h> //Internet addresses
#include<string.h> // memset
#include<arpa/inet.h>//inet_addr

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 

#include <fcntl.h> 
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
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //connecting
    if(connect(sfd,(struct sockaddr *) &addr,sizeof(addr))==-1){
        perror("Connection Failed !");
        exit(0);
    }
     
     char filecontent[1000];
     int f1=open("P.c",O_RDONLY);
     int s1=read(f1,filecontent,sizeof(filecontent));
     filecontent[s1]='\0';

   if(send(sfd,filecontent,sizeof(filecontent),0)==-1){
       perror("Sending failed");
       exit(0);
   }
   else{
       printf("File sent successfully\n");
   }
    
    char buffer2[100];
     //Waiting for reply
    memset(buffer2,'\0',sizeof(buffer2));
    if(recv(sfd,buffer2,sizeof(buffer2),0)==-1){
        perror("Message not recieved");
        exit(0);
    } 
	printf("message recieved by client:%s",buffer2);

     close(sfd);

}