#include<sys/socket.h>
#include<netinet/in.h> //Internet addresses
#include<string.h> // memset
#include<arpa/inet.h>//inet_addr

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include<sys/wait.h>
#include<signal.h>

#include<sys/un.h>
#include <errno.h>


#include<fcntl.h> 
#include <unistd.h>

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

#define BUF_LEN 1024
int rsfd_recieved;

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
        else{
            printf("usfd recieved\n");
        }

        cmsg = CMSG_FIRSTHDR(&msg);

        memcpy (fds, (int *) CMSG_DATA(cmsg),  sizeof(int));

        return fds;
}

void print_ipheader(struct iphdr* ip)
{
	cout<<"------------------------\n";
	cout<<"Printing IP header....\n";
	cout<<"IP version:"<<(unsigned int)ip->version<<endl;
	cout<<"IP header length:"<<(unsigned int)ip->ihl<<endl;
	
	cout<<"Type of service:"<<(unsigned int)ip->tos<<endl;
	cout<<"Total ip packet length:"<<ntohs(ip->tot_len)<<endl;
	cout<<"Packet id:"<<ntohs(ip->id)<<endl;
	cout<<"Time to leave :"<<(unsigned int)ip->ttl<<endl;
	cout<<"Protocol:"<<(unsigned int)ip->protocol<<endl;
	cout<<"Check:"<<ip->check<<endl;
	cout<<"Source ip:"<<inet_ntoa(*(in_addr*)&ip->saddr)<<endl;
	//printf("%pI4\n",&ip->saddr );
	cout<<"Destination ip:"<<inet_ntoa(*(in_addr*)&ip->daddr)<<endl;
	cout<<"End of IP header\n";
	cout<<"------------------------\n";
}

int main()
{ 
    
    struct sockaddr_un uaddr;
    int usfd;
    usfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (usfd == -1){
        printf("Error while creating the socket \n");
        exit(0);
    }
    int enable = 1;
    if(setsockopt(usfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
            perror("setsockopt(SO_REUSEADDR) failed");
    }
    bzero(&uaddr,sizeof(uaddr)); 
    memset(&uaddr, 0, sizeof(struct sockaddr_un));

    uaddr.sun_family = AF_UNIX;
    strncpy(uaddr.sun_path, "/tmp/passing-fd.socket1", sizeof(uaddr.sun_path)-1);

    

    if(connect(usfd, (struct sockaddr *) &uaddr, sizeof(struct sockaddr_un)) ==-1){
        perror("Connection Failed !");
        exit(0);
    }

   

    int * fd=recv_fd(usfd);
    rsfd_recieved=fd[0];

    char buf[BUF_LEN];
	struct sockaddr_in client;
	socklen_t clilen=sizeof(client);
	cout<<"receive"<<endl;
	recvfrom(rsfd_recieved,buf,BUF_LEN,0,(sockaddr*)&client,(socklen_t*)clilen);

	cout<<buf<<endl;
	 struct iphdr *ipp;
  	ipp=(struct iphdr*)buf;
  	
  	 print_ipheader(ipp);
    
    int fds[1] ;
    fds[0]=rsfd_recieved;

    send_fd(usfd,fds);


}