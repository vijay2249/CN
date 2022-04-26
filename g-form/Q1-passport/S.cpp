#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include <sys/un.h>
#include <sys/wait.h>
#include <sys/socket.h>

#include <errno.h>
#include <pthread.h>

#include<netinet/in.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<errno.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/udp.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<bits/stdc++.h>

using namespace std;
int usfd[3];
int unsfd[3] ;
struct sockaddr_un addr;


int * recv_fd(int socket) {
        int *fds = (int*) malloc(sizeof(int));
        struct msghdr msg = {0};
        struct cmsghdr *cmsg;
        char buf[CMSG_SPACE( sizeof(int))], dup[256];
        memset(buf, '\0', sizeof(buf));
        struct iovec io = { .iov_base = &dup, .iov_len = sizeof(dup) };

        msg.msg_iov = &io;
        msg.msg_iovlen = 1;
        msg.msg_control = buf;
        msg.msg_controllen = sizeof(buf);

        if (recvmsg (socket, &msg, 0) < 0)
            {
                perror ("Failed to receive message");
                exit(0);
                }

        cmsg = CMSG_FIRSTHDR(&msg);

        memcpy (fds, (int *) CMSG_DATA(cmsg),  sizeof(int));

        return fds;
}


void send_fd(int socket, int *fds)  // send fd by socket
{
        struct msghdr msg = {0};
        struct cmsghdr *cmsg;
        char buf[CMSG_SPACE( sizeof(int))], dup[256];
        memset(buf,'\0', sizeof(buf));
        struct iovec io = { .iov_base = &dup, .iov_len = sizeof(dup) };

        msg.msg_iov = &io;
        msg.msg_iovlen = 1;
        msg.msg_control = buf;
        msg.msg_controllen = sizeof(buf);

        cmsg = CMSG_FIRSTHDR(&msg);
        cmsg->cmsg_level = SOL_SOCKET;
        cmsg->cmsg_type = SCM_RIGHTS;
        cmsg->cmsg_len = CMSG_LEN( sizeof(int));

        memcpy ((int *) CMSG_DATA(cmsg), fds,  sizeof (int));

        if (sendmsg (socket, &msg, 0) < 0)
               { perror("Failed to send message");
                exit(0);}
        else {
                 printf("FD sent \n");
            }
}





int main(){

    //Creating rsfd Socket
    int rsfd=socket(AF_INET,SOCK_RAW,IPPROTO_UDP);

	perror("socket");
	int optval=1;
	setsockopt(rsfd, IPPROTO_IP, SO_BROADCAST, &optval, sizeof(int));//IP_HDRINCL
	cout<<"opt"<<endl;
	struct sockaddr_in client;
	client.sin_family=AF_INET;
	client.sin_addr.s_addr=inet_addr("127.0.0.1");

	char buff[]="hello";	
	client.sin_addr.s_addr=INADDR_ANY;

	unsigned int client_len=sizeof(client);
	cout<<"sending"<<endl;
  	sendto(rsfd,buff,strlen(buff)+1,0,(struct sockaddr*)&client,sizeof(client));
  	perror("send");
  	



    //USFD 1 Socket
    usfd[0] = socket(AF_UNIX, SOCK_STREAM, 0);
    if (usfd[0] == -1){
        printf("Error while creating the socket \n");
        exit(0);
    }
    int enable = 1;
    if(setsockopt(usfd[0], SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
            perror("setsockopt failed");
    }

    bzero(&addr,sizeof(addr)); 
    memset(&addr, 0, sizeof(struct sockaddr_un));

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "/tmp/passing-fd.socket ", sizeof(addr.sun_path)-1);


    if(bind(usfd[0], (struct sockaddr *) &addr, sizeof(struct sockaddr_un))==-1)
    {
        perror("Error while binding \n");
        exit(0);
        }
    if(listen(usfd[0], 2)==-1){
        perror("Error while Listing \n");
        exit(0);
    }

    unsfd[0] = accept(usfd[0], NULL, NULL);
    if(unsfd[0]==-1){
        perror("error in accept");
        exit(0);
    }
    else{
        printf("USFD Connected\n");
    }

    // //USFD socket 2
      usfd[1] = socket(AF_UNIX, SOCK_STREAM, 0);
    if (usfd[1] == -1){
        printf("Error while creating the socket \n");
        exit(0);
    }
     enable = 1;
    if(setsockopt(usfd[1], SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
            perror("setsockopt failed");
    }

    bzero(&addr,sizeof(addr)); 
    memset(&addr, 0, sizeof(struct sockaddr_un));

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "/tmp/passing-fd.socket2", sizeof(addr.sun_path)-1);

    if(bind(usfd[1], (struct sockaddr *) &addr, sizeof(struct sockaddr_un))==-1)
    {
        perror("Error while binding \n");
        exit(0);
        }
    if(listen(usfd[1], 2)==-1){
        perror("Error while Listing \n");
        exit(0);
    }

    unsfd[1] = accept(usfd[1], NULL, NULL);
    if(unsfd[1]==-1){
        perror("error in accept");
        exit(0);
    }
    else{
        printf("USFD Connected\n");
    }

    //USDF socket 3

      usfd[2] = socket(AF_UNIX, SOCK_STREAM, 0);
    if (usfd[2] == -1){
        printf("Error while creating the socket \n");
        exit(0);
    }
     enable = 1;
    if(setsockopt(usfd[2], SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
            perror("setsockopt failed");
    }

    bzero(&addr,sizeof(addr)); 
    memset(&addr, 0, sizeof(struct sockaddr_un));

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "/tmp/passing-fd.socket3", sizeof(addr.sun_path)-1);

    if(bind(usfd[2], (struct sockaddr *) &addr, sizeof(struct sockaddr_un))==-1)
    {
        perror("Error while binding \n");
        exit(0);
        }
    if(listen(usfd[2], 2)==-1){
        perror("Error while Listing \n");
        exit(0);
    }

    unsfd[2] = accept(usfd[2], NULL, NULL);
    if(unsfd[2]==-1){
        perror("error in accept");
        exit(0);
    }
    else{
        printf("USFD Connected\n");
    }

    //Sending the rsfd to each Server A,B,C

    int fds[1] ;
    fds[0]=rsfd;
    send_fd(unsfd[0],fds);
    int * fd=recv_fd(unsfd[0]);
    cout<<fd[0];//rsfd Recieved Back
    
    fds[0]=fd[0];
    send_fd(unsfd[1],fds);
    int * fd2=recv_fd(unsfd[1]);
    cout<<fd2[0];//rsfd Recieved Back

    fds[0]=fd2[0];
    send_fd(unsfd[2],fds);
    int * fd3=recv_fd(unsfd[2]);
    cout<<fd3[0];//rsfd Recieved Back


    //Client Connection TCP Socket 
    int sfd;
    sfd=socket(AF_INET,SOCK_STREAM,0);
    if(sfd==-1){
        perror("Socket creation failed");
        exit(0);
    }
    struct sockaddr_in addr;

    addr.sin_family=AF_INET;
    addr.sin_port=htons(8080);
    addr.sin_addr.s_addr=inet_addr("127.0.0.1");

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

    fds[0]=nsfd;
    send_fd(unsfd[0],fds);//Send the nsfd using usfd created above to process A



}