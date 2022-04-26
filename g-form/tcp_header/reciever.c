#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stddef.h>
#include <fcntl.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>

#define h_addr h_addr_list[0]
struct sockaddr_in source, dest;
struct sockaddr_in serv_addr, cli_addr;

unsigned short chksum(void *b, int len)
{
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char *)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

void print_ip_header(char *buff)
{
    unsigned short iphdrlen;
    struct iphdr *iph = (struct iphdr *)buff;
    iphdrlen = iph->ihl * 4;
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;

    printf("\n			IP Header\n");
    printf("IP Version        : %d\n", (unsigned int)iph->version);
    printf("IP Header Length  : %d DWORDS or %d Bytes\n", (unsigned int)iph->ihl, ((unsigned int)(iph->ihl)) * 4);
    printf("Type Of Service   : %d\n", (unsigned int)iph->tos);
    printf("IP Total Length   : %d  Bytes(Size of Packet)\n", ntohs(iph->tot_len));
    printf("Identification    : %d\n", ntohs(iph->id));
    printf("TTL      		  : %d\n", (unsigned int)iph->ttl);
    printf("Protocol 		  : %d\n", (unsigned int)iph->protocol);
    printf("Checksum 		  : %d\n", ntohs(iph->check));
    printf("Source IP         : %s\n", inet_ntoa(source.sin_addr));
    printf("Destination IP    : %s\n", inet_ntoa(dest.sin_addr));
}
void print_tcp_header(char *buff)
{
    printf("\n			TCP HEADER \n");
    unsigned short iphdrlen;
    struct iphdr *iph = (struct iphdr *)buff;
    iphdrlen = iph->ihl * 4;
    struct tcphdr *tcph = (struct tcphdr *)(buff + sizeof(struct iphdr));

    printf("Source port is			  : %d\n", ntohs(tcph->source));
    printf("Destination port is		  : %d\n", ntohs(tcph->dest));
    printf("Sequence number is 		  : %d\n", (unsigned int)tcph->seq);
    printf("Acknowledgement number is : %d\n", (unsigned int)tcph->ack_seq);
    printf("Checksum 				  : %d\n", ntohs(tcph->check));
    printf("Window size is			  : %d\n", (unsigned short int)tcph->window);
    printf("Urgptr is 				  : %d\n", (unsigned short int)tcph->urg_ptr);
}

int main()
{
    int sfd;
    memset(&cli_addr, 0, sizeof(cli_addr));
    cli_addr.sin_family = AF_INET;
    cli_addr.sin_port = htons(9090);
    cli_addr.sin_addr.s_addr = inet_addr("127.0.0.2");

    sfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    perror("socket ");
    bind(sfd, (struct sockaddr *)&cli_addr, sizeof(cli_addr));
    perror("bind ");

    int i = 0;
    char buff[1024];
    int len = sizeof(serv_addr);
    int r = recvfrom(sfd, buff, 1024, 0, (struct sockaddr *)&serv_addr, &len);
    perror("recvfrom");

    // extracting ip header, it lies at starting of the buffer we recieved
    struct iphdr *iph = (struct iphdr *)buff;
    // extracting tcp header, it lies after ip header, so we start after ip header
    struct tcphdr *tcph = (struct tcphdr *)(buff + sizeof(struct iphdr));

    printf("Now we print IP and TCP Header\n");
    print_ip_header(buff);
    print_tcp_header(buff);

    // converting ip address from number to . format
    char sender_addr[100];
    struct in_addr in;
    in.s_addr = iph->saddr;
    strcpy(sender_addr, inet_ntoa(in));
    printf("\nPayload is:\n");
    printf("'%s' and this message is recieved from : %s\n", buff + sizeof(struct iphdr) + sizeof(struct tcphdr), sender_addr);

    return 0;
}