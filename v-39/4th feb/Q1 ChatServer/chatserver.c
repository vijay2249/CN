#include<sys/socket.h> 
#include<netinet/in.h> //Internet addresses
#include<sys/wait.h>
#include<arpa/inet.h>

#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<fcntl.h> 
#include<string.h> // memset
#include <sys/select.h>


#define count 5
#define PORT 8080

int main(){
  
    
    //Socket Creation
    int sfd;
    sfd=socket(AF_INET,SOCK_STREAM,0);
    if(sfd==-1){
    perror("Socket creation failed\n");
    exit(0);
    }

    //Binding
    struct sockaddr_in addr;
    addr.sin_family=AF_INET; //IPv4 Internet domain
    addr.sin_port=htons(PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(bind(sfd,(struct sockaddr *)&addr,sizeof(addr))==-1){
    perror("binding failed\n");
    exit(0);    
    }
  
    if(listen(sfd,4)==-1){
        perror("error in listen\n");
        exit(0);
    }
    
    

    //Define fdset collection
    fd_set fdset;
    int nsfds[count];
    memset(nsfds,-1,sizeof(nsfds));
    int p=0;
     while(1){
         sleep(1);
         FD_ZERO(&fdset);//Clear the fdset array to 0
         FD_SET(sfd,&fdset);
		 int maxfd=sfd;
         for(int i=0;i<count;i++)
		{
			if(nsfds[i]==-1)continue;
			FD_SET(nsfds[i],&fdset);
            
			if(nsfds[i]>maxfd)
			{
				maxfd=nsfds[i];
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
            
            if(FD_ISSET(sfd,&fdset)){
                 int addrlen=sizeof(addr);
                        int nsfd=accept(sfd,(struct sockaddr *)&addr, (socklen_t *)&addrlen);
                        if(nsfd==-1){
                        perror("error in accept");
                        exit(0);
                        }
                       else{
                        nsfds[p++]=nsfd;
                        printf("Connection for client %d accepted \n",p);
                       }
                                   
            }
            
            for(int i=0;i<count;i++){
               
                    if(nsfds[i]==-1)continue;
                    if(FD_ISSET(nsfds[i],&fdset)){
                           char buffer[1024]; 
                           memset(buffer,'\0',sizeof(buffer));
		                   int k=recv(nsfds[i],buffer,sizeof(buffer),0);
                           if(k>0){
                               printf("Message recieved\n");
                           }
		                  // printf("Message Recieced from client %d :%s\n",i,buffer);

                            for(int j=0;j<count;j++){
                                    if(nsfds[j]==-1 || j==i)continue;
                                    int s = send(nsfds[j],buffer,sizeof(buffer),0);
		                            if(s==-1)
		                            {
		                            	write(1,"Msg not sent\n",sizeof("Msg not sent\n"));
		                            	exit(0);
		                            }
                                    else{
                                        printf("Message sent to Client - %d \n",j+1);
                                    }    

                            }
                            break;               
                       
                    }

            }
  
        }


      }
   

}