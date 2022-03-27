#include<sys/socket.h> 
#include<netinet/in.h> //Internet addresses
#include<string.h> // memset
#include <sys/wait.h>
#include<arpa/inet.h>

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 

#define PORT 8080
#define PORT_server1 6060
#define PORT_server2 7070


struct args {//Passing arguments to Thread
    int sfd;    //sfd of corresponding Special Server
    int nsfd;   //nsfd of corresponding Client 
};

void* thread_of_Mediator(void * arg)
{
    
    int sfd1=((struct args*)arg)->sfd;   //sfd of corresponding Special Server
    int m_nsfd1=((struct args*)arg)->nsfd;  //nsfd of corresponding Client 

     	char buffer1[1024]; 

   	    
		//Recieving message from Server 1
        memset(buffer1,'\0',sizeof(buffer1));
		recv(sfd1,buffer1,sizeof(buffer1),0);
	     //Sending back to C1
         
        int s1 = send(m_nsfd1,buffer1,sizeof(buffer1),0);
		if(s1==-1)
		{
			write(1,"send error\n",sizeof("send error\n"));
			exit(0);
		}

        //Recieving message from Client 1
        memset(buffer1,'\0',sizeof(buffer1));
		recv(m_nsfd1,buffer1,sizeof(buffer1),0);
        //Sending to server
         
         s1 = send(sfd1,buffer1,sizeof(buffer1),0);
		if(s1==-1)
		{
			write(1,"send error\n",sizeof("send error\n"));
			exit(0);
		}
      
	
}
void specialserver_1(int m_nsfd1){
      //Special Server 1 Socket
    int sfd1;
	sfd1 = socket(AF_INET,SOCK_STREAM,0);
	if(sfd1==-1)
	{
		printf("socket creation error\n");
		exit(0);
	}
	struct sockaddr_in addr1;
	addr1.sin_family = AF_INET;
	addr1.sin_port = htons(PORT_server1);//Connecting to special server 1 port
	addr1.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	if(connect(sfd1,(struct sockaddr*) &addr1,sizeof(addr1))!=0)
	{
		printf("Connect failed!\n");
		exit(0);
	}
    
    struct args *parameters = (struct args *)malloc(sizeof(struct args));
    


    parameters->sfd =sfd1;
    parameters->nsfd=m_nsfd1;
    pthread_t thread1;
    pthread_create(&thread1,NULL,thread_of_Mediator,(void *)parameters);
    pthread_join(thread1, NULL);

}


void specialserver_2(int m_nsfd2){
    //Special Server 2 Socket
     int sfd2;
	sfd2 = socket(AF_INET,SOCK_STREAM,0);
	if(sfd2==-1)
	{
		printf("socket creation error\n");
		exit(0);
	}
	struct sockaddr_in addr2;
	addr2.sin_family = AF_INET;
	addr2.sin_port = htons(PORT_server2);//Connecting to special server 2 port
	addr2.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	if(connect(sfd2,(struct sockaddr*) &addr2,sizeof(addr2))!=0)
	{
		printf("Connect failed!\n");
		exit(0);
	}
    
    struct args *parameters = (struct args *)malloc(sizeof(struct args));
    parameters->sfd =sfd2;
    parameters->nsfd=m_nsfd2;
    pthread_t thread2;
    pthread_create(&thread2,NULL,thread_of_Mediator,(void *)parameters);
    pthread_join(thread2, NULL);


}



int main(){
    
    // Mediator Socket creation
    int m_sfd;
    m_sfd=socket(AF_INET,SOCK_STREAM,0);
    if(m_sfd==-1){
        perror("Socket creation failed");
        exit(0);
    }

    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(PORT);
    addr.sin_addr.s_addr=inet_addr("127.0.0.1");

    if(bind(m_sfd,(struct sockaddr *)&addr,sizeof(addr))==-1){
        perror("binding failed");
        exit(0);    
    }
    if(listen(m_sfd,1)==-1){
        perror("error in listen");
        exit(0);
    }
   

   while(1){

    int addrlen1=sizeof(addr);
    int m_nsfd=accept(m_sfd,(struct sockaddr *)&addr, (socklen_t *)&addrlen1);
    if(m_nsfd==-1){
        perror("error in accept");
        exit(0);
    }
    else{
        printf("Socket accepted request\n");
    }

   
        char buffer[100];
        memset(buffer,'\0',sizeof(buffer));
	    recv(m_nsfd,buffer,sizeof(buffer),0);
	    printf("Redirecting to Server : %s\n",buffer);
        if(buffer[0]=='1'){
             specialserver_1(m_nsfd);
        }
        else if(buffer[0]=='2'){
             specialserver_2(m_nsfd);
        }
       
    sleep(5);
   }
   
  			
}