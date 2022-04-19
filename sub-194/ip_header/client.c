#include<time.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/select.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/un.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<errno.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/udp.h>
#include<sys/ipc.h>
#include<sys/msg.h>

int main(){

    //creating a raw socket
    int rsfd=socket(AF_INET,SOCK_RAW,2);
    perror("creating socket");

    int optval = 1;
    setsockopt(rsfd, IPPROTO_IP, SO_BROADCAST, &optval, sizeof(int));

    char buf[1024];

    //to use in receive statement --- stores the address of sender in it.
    struct sockaddr_in addr;
	socklen_t len = sizeof(addr);

    recvfrom(rsfd,buf,sizeof(buf),0,(struct sockaddr*)&addr,(socklen_t*)&len);
	perror("receiving");

    struct iphdr *ip;
    ip=(struct iphdr*)buf;
    printf("The message from server is: %s\n",(buf+(ip->ihl)*4));

    printf("IP data is: \n");
    
    printf("The version is: %d\n",ip->version);
    printf("IP Check: %d\n",ip->check);
    printf("Destination Address: %d\n",ip->daddr);
    printf("Fragment offset is: %d\n",ip->frag_off);
    printf("IP Header Length is: %d\n",ip->ihl);
    printf("The protocol is: %d\n",ip->protocol);
    printf("Source Address is: %d\n",ip->saddr);
    printf("Type of service is: %d\n",ip->tos);
    printf("Total length is: %d\n",ip->tot_len);
    printf("Time to live is: %d\n",ip->ttl);
    printf("Version is: %d\n",ip->version);

    return 0;
}