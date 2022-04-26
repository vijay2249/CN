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
#include<bits/stdc++.h>
using namespace std;

#define BUF_LEN 1024

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
	int s = socket (PF_INET, SOCK_RAW, 1);
	int n = 0 ;
	int one = 1;
    	const int *val = &one;
    	if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
    	cout<<"Not set";
    	char buff[4096];
    	struct sockaddr_in sin,sin2; 
    	socklen_t len = sizeof(sin2);
   	    memset(&sin,0,sizeof(sin));
    	sin.sin_family = AF_INET;
    	sin.sin_port = htons (8081);
    	sin.sin_addr.s_addr = htonl(INADDR_ANY);
    	bind(s,(struct sockaddr *) &sin,sizeof(sin));
    	recvfrom(s,buff,4096,0,(struct sockaddr *) &sin,&len);
    	
    	
    	struct iphdr *iph = (struct iphdr*)buff;

        //convert ip address from number to . format
        char buff1[100];
        struct sockaddr_in in,din;
        in.sin_addr.s_addr = iph->saddr;
        din.sin_addr.s_addr=iph->daddr;
        strcpy(buff1, inet_ntoa(in.sin_addr));
        printf("version:%d\n",iph->version);
        printf("time to live:%d\n",iph->ttl);
        printf("id:%d\n",iph->id);
        printf("protocol:%d\n",iph->protocol);
        printf("foreign IP:%s\n",inet_ntoa(in.sin_addr));
        printf("foreign IP:%s\n",inet_ntoa(din.sin_addr));
        // printf("data : %s\n", buff + sizeof(struct iphdr) + sizeof(struct tcphdr));
}