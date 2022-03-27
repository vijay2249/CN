#include<sys/socket.h> 
#include<netinet/in.h> //Internet addresses
#include<sys/wait.h>
#include<arpa/inet.h>

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include<fcntl.h> 
#include<string.h> // memset

#include<pthread.h>
#include<sys/select.h>


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

		 printf("\nThreaded Communication :\nMsg from client: %s\n",buffer);

    
        char *hello = "Hello thread from Server";
        if(send(nsfd, (const char *)hello, strlen(hello),0)==-1){
        perror("Sending failed");
        exit(0);
        }
        // else{
        //     printf("Msg sent to client 3\n");
        // }
    }
	
}

int main(){
  
    int sfd[count];

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

    //Define fdset collection
    fd_set fdset;

     while(1){

         FD_ZERO(&fdset);//Clear the fdset array to 0
		 int maxfd=-1;
         for(int i=0;i<count;i++)
		{
			if(sfd[i]==-1)continue;
			FD_SET(sfd[i],&fdset);
            
			if(sfd[i]>maxfd)
			{
				maxfd=sfd[i];
			}
		}

        int n=select(maxfd+1,&fdset,NULL,NULL,NULL);
        if(n==-1)	
		{
			perror("select error");
		}
		else if(n==0)//Timeout
		{
			printf("time out\n");
		}
		else//Ready event generation
		{
            
            if(FD_ISSET(sfd[0],&fdset))	
			{
                 int addrlen=sizeof(addr[0]);
                 int nsfd=accept(sfd[0],(struct sockaddr *)&addr[0], (socklen_t *)&addrlen);

                     if(nsfd==-1){
                     perror("error in accept");
                     exit(0);
                        }
                    else{
                        printf("Connection fork accepted request\n");
                    }

                   int c1=fork();
                        if(c1>0){
                            close(nsfd);
                            close(sfd[0]);
                            sfd[0]=-1;
                          }
                            else{
                             close(sfd[1]);
                             close(sfd[2]);
                            dup2(nsfd,1);
                            dup2(nsfd,0);
                                //Transfer control to Server S3
                                 char *args[]={"./s1",NULL};
                                 execvp(args[0],args);
                            }   

            }
            else if(FD_ISSET(sfd[1],&fdset)){
                printf("\nConnectionLess Communication :\n");
                char buffer[100];
                
                struct sockaddr_in  cliaddr;
				int len, n;
                memset(&cliaddr, 0, sizeof(cliaddr));
                len = sizeof(cliaddr);  

                n = recvfrom(sfd[1], (char *)buffer, 100, MSG_WAITALL, 
                   (struct sockaddr *) &cliaddr,&len);
                buffer[n] = '\0';
                printf("Msg from Client : %s\n", buffer);
                
                char *hello = "Hello from server";
                sendto(sfd[1], (const char *)hello, strlen(hello), 
                MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
                len);
                // printf("Hello message sent by sever to C2.\n");   

            }
            else if(FD_ISSET(sfd[2],&fdset)){
                
                int addrlen=sizeof(addr[2]);
                 int nsfd=accept(sfd[2],(struct sockaddr *)&addr[2], (socklen_t *)&addrlen);

                if(nsfd==-1){
                     perror("error in accept");
                     exit(0);
                        }
                else{
                        printf("Threaded Socket accepted request\n");
                    }
                
                pthread_t thread;
				pthread_create(&thread,NULL,server_for_C3,(void *)&nsfd);
                // close(sfd[2]);
                sfd[2]=-1;
                //wait(NULL);      

            }



            
        }


      }
   

}