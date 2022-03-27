#include<sys/socket.h> 
#include<netinet/in.h> //Internet addresses
#include<string.h> // memset
#include <sys/wait.h>
#include<arpa/inet.h>

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 


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
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
    //addr.sin_addr.s_addr=INADDR_ANY;
    //  if(inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr)==-1) 
	// { 
	// 	perror("Invalid Address");
	// 	exit(0); 
	// } 

    //binding
    if(bind(sfd,(struct sockaddr *)&addr,sizeof(addr))==-1){
        perror("binding failed");
        exit(0);    
    }
     
    
     struct sockaddr_in localaddr;
     int loc_len=sizeof(localaddr);
     getsockname(sfd,(struct sockaddr * )&localaddr,&loc_len);

	

    printf("Local IP address is: %s\n", inet_ntoa(localaddr.sin_addr));
     printf("Local port is: %d\n", (int)ntohs(localaddr.sin_port));

    //listening
    if(listen(sfd,2)==-1){
        perror("error in listen");
        exit(0);
    }
    

   while(1){
        printf("---------------------------------------------------------------------\n");
    //accept
    int addrlen=sizeof(addr);
    int nsfd=accept(sfd,(struct sockaddr *)&addr, (socklen_t *)&addrlen);
    if(nsfd==-1){
        perror("error in accept");
        exit(0);
    }
    

    struct sockaddr_in foreignaddr;
	socklen_t len1 = sizeof(foreignaddr);
	getpeername(nsfd,(struct sockaddr *)&foreignaddr,&len1);
	char foreign_IP[16];
	inet_ntop(AF_INET, &foreignaddr.sin_addr, foreign_IP, sizeof(foreign_IP));

	printf("Foreign ip address: %s\n",foreign_IP);
	printf("Foreign port : %d\n\n",ntohs(foreignaddr.sin_port));



    int c=fork();
    if(c>0){
         close(nsfd);
         wait(NULL);
         }
    else{
    
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
       return 0;
       }
    
    }
    close(sfd);
    
    
}