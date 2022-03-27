#include<sys/socket.h> 
#include<netinet/in.h> //Internet addresses
#include<sys/wait.h>
#include<arpa/inet.h>

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include<fcntl.h> 
#include<string.h> // memset
#include<poll.h>
#include<pthread.h>

#define count 3

void* server_for_C3(void * arg)
{
	 int *ptr;
    int nsfd;
    ptr = (int *) arg;
    nsfd = *ptr;
    char buffer[100];
	while(1){
         memset(buffer,'\0',sizeof(buffer));
		recv(nsfd,buffer,sizeof(buffer),0);
		//printf("Msg from client3: %s\n",buffer);
         for(int i=0;buffer[i]!='\0';i++){
          if(buffer[i]>='a' && buffer[i]<='z'){
              buffer[i]=buffer[i]-32;
          }
     }
        // if(buffer[0]=='D'){close(nsfd);break;}
		send(nsfd,buffer,sizeof(buffer),0);
    }
	
}

int main(){
  
    int sfd[count];
    int nsfd[count];
    int socket_type[count]={SOCK_STREAM,SOCK_DGRAM,SOCK_STREAM};
    struct sockaddr_in addr[count];
    int port_number[count]={6060,7070,8080};

    for(int i=0;i<count;i++){
        //Socket Creation
        sfd[i]=socket(AF_INET,socket_type[i],0);
        if(sfd[i]==-1){
        perror("Socket creation failed\n");
        exit(0);
        }
        //Binding
        addr[i].sin_family=AF_INET; //IPv4 Internet domain
        addr[i].sin_port=htons(port_number[i]);
        addr[i].sin_addr.s_addr = inet_addr("127.0.0.1");

        if(bind(sfd[i],(struct sockaddr *)&addr[i],sizeof(addr[i]))==-1){
        perror("binding failed\n");
        exit(0);    
        }
        //Listen if It is connection oriented
        if(socket_type[i]==SOCK_STREAM)
        {
                if(listen(sfd[i],2)==-1){
                perror("error in listen\n");
                exit(0);
                }
        }

    }

     struct pollfd pfds[3];
     while(1){
        
        for(int i=0;i<count;i++){
            pfds[i].fd=sfd[i];
	        pfds[i].events=POLLIN;
	        pfds[i].revents=0;
        }
	   
        

        int c=poll(pfds,count,500);
		if(c==-1)
		perror("poll error");
		else{
			if(pfds[0].revents && POLLIN)
			{	
                 int addrlen=sizeof(addr[0]);
                 nsfd[0]=accept(sfd[0],(struct sockaddr *)&addr[0], (socklen_t *)&addrlen);

                     if(nsfd[0]==-1){
                     perror("error in accept");
                     exit(0);
                        }
                    else{
                        printf("Socket 1 accepted request\n");
                    }

                   int c1=fork();
                        if(c1>0){
                            close(nsfd[0]);
                           //  wait(NULL);
                            // close(sfd[0]);
                          }
                            else{
                             close(sfd[1]);
                             close(sfd[2]);
                            dup2(nsfd[0],1);
                            dup2(nsfd[0],0);
                                //Transfer control to Server S3
                                 char *args[]={"./s1",NULL};
                                 execvp(args[0],args);
                            }   
                
			}
            if(pfds[1].revents && POLLIN)
			{	
                //Connection Less Client C2
                char buffer[100];
                char *hello = "Hello from server";
                struct sockaddr_in  cliaddr;
				int len, n;
                memset(&cliaddr, 0, sizeof(cliaddr));
                len = sizeof(cliaddr);  

                n = recvfrom(sfd[1], (char *)buffer, 100, MSG_WAITALL, 
                   (struct sockaddr *) &cliaddr,&len);
                buffer[n] = '\0';
                printf("Client : %s\n", buffer);

                sendto(sfd[1], (const char *)hello, strlen(hello), 
                MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
                len);
                printf("Hello message sent by sever to C2.\n");


			}
            if(pfds[2].revents && POLLIN)
			{	
                 int addrlen=sizeof(addr[2]);
                  nsfd[2]=accept(sfd[2],(struct sockaddr *)&addr[2], (socklen_t *)&addrlen);

                if(nsfd[2]==-1){
                     perror("error in accept");
                     exit(0);
                        }
                else{
                        printf("Socket 3 accepted request\n");
                    }
                
                pthread_t thread;
				pthread_create(&thread,NULL,server_for_C3,(void *)&nsfd[2]);
                wait(NULL);
			}

          }
      
      }
   

}