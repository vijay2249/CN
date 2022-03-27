//As both Server and Client may be present at different locations
//I sent whole file as a buffer and server recieves it and create a new Pnew.c file
//and compile it 

#include<sys/socket.h> 
#include<netinet/in.h> //Internet addresses
#include<string.h> // memset
#include <sys/wait.h>
#include<arpa/inet.h>

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 

#include<stdlib.h>
#include<fcntl.h> 

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
	

    //binding
    if(bind(sfd,(struct sockaddr *)&addr,sizeof(addr))==-1){
        perror("binding failed");
        exit(0);    
    }
     
    //listening
    if(listen(sfd,2)==-1){
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
    
    char buffer[623]; 
        memset(buffer,'\0',sizeof(buffer));
		int valread = recv(nsfd,buffer, sizeof(buffer),0); 
		printf("File recieved by server from client");

    //creating new file Pnew.c and storing the content into the file	
     int p1=open("Pnew.c",O_RDWR | O_CREAT);
      write(p1,buffer,sizeof(buffer));
    
    //Creating the 'gcc Pnew.c -o P' command
    char command[]="gcc ";
    strcat(command,"Pnew.c");
    strcat(command," -o P");

    //printf("\n%s\n",command);

    system(command);//Compiling P.c to P.exe
    
    int c=fork();
    if(c>0){
        
         wait(NULL);
         
        int out1=open("Pout.txt",O_RDONLY);
        int out2=open("OT.txt",O_RDONLY);

        char c1[100];
            int s1=read(out1,c1,sizeof(c1));
            c1[s1]='\0';
        char c2[100];
            int s2=read(out2,c2,sizeof(c2));
            c2[s2]='\0';
        
        printf("\nPout.txt :\n%s\n",c1);
        printf("OT.txt :\n%s\n\nCompairing .........\n\n",c2);

        if(strcmp(c1,c2)==0) {
            char  pass[]="test cases passed\n";
		    send(nsfd,pass,sizeof(pass),0); 
        }
        else{
            char fail[]="test cases failed\n";
		    send(nsfd,fail,sizeof(fail),0); 
        }
		close(nsfd);
        close(sfd);
        }
    else{
    
        int itfd=open("IT.txt",O_RDONLY);
        int poutfd=open("Pout.txt",O_WRONLY);

        dup2(itfd,0);
        dup2(poutfd,1);

        close(nsfd);
        close(sfd);

        char *args[]={"./P",NULL};
        execvp(args[0],args);

       }
    
    
}