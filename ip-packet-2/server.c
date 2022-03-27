#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#include<sys/select.h>
#include<sys/socket.h>
#include<sys/ipc.h>
#include<netinet/in.h>
#include<pthread.h>
#include<signal.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<sys/un.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<errno.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/tcp.h>
#include<netinet/udp.h>
#include<sys/msg.h>
#include <netinet/ip_icmp.h>

unsigned short chksum(void *b, int length){
    unsigned short *buffer = b;
    unsigned int sum = 0;
    unsigned short result;
    for(sum = 0;length > 1; length -= 2) sum += *buffer;
    if(length == 1) sum += *(unsigned char *)buffer;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}


int main(){
    
    //creating rsfd
    int rsfd=socket(AF_INET,SOCK_RAW,2);
    perror("creating socket");
    int optval = 1;
    setsockopt(rsfd, IPPROTO_IP, SO_BROADCAST, &optval, sizeof(int));

    //targeting client
    struct sockaddr_in client;
    client.sin_family=AF_INET;
    client.sin_addr.s_addr=INADDR_ANY;

    printf("Enter content to send to client\n");
    char buffer[1024];
    scanf("%[^\n]%*c",buffer);

    // content sending without any modification to the IP packet
    // sendto(rsfd,buffer,sizeof(buffer),0,(struct sockaddr*)&client,sizeof(client));
    // perror("sending");
    // printf("\n\n\n");


    printf("sending modified ip data packet\n");
    // modifying the IP packet and sending the modified packet
    struct iphdr *iph = (struct iphdr *)buffer;
    memset(buffer, 0, 1024);
    iph->ihl = 5;
    iph->tos = 0;
    iph->version = 4;
    iph->id = htons(44444);
    iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr) + strlen(buffer);
    iph->frag_off = 0;
    iph->ttl = 255;
    iph->protocol = IPPROTO_TCP;
    iph->check =chksum((unsigned short *)buffer, iph->tot_len);
    iph->saddr = inet_addr("8.8.8.8");
    iph->daddr = client.sin_addr.s_addr;
    if(sendto(rsfd, buffer, iph->tot_len, 0, (struct sockaddr *)&client, sizeof(client))<0){
        perror("sendto fail..");
        exit(1);
    }else{
        printf("packet of length: %d bytes sent..\n", iph->tot_len);
    }

    return 0;
}
