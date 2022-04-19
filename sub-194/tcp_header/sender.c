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

int main()
{
	int sd;
	char buffer[1024], *payload;

	struct iphdr *iph = (struct iphdr *)buffer;
	struct tcphdr *tcph = (struct tcphdr *)(buffer + sizeof(struct iphdr));
	memset(buffer, 0, 1024);

	payload = buffer + sizeof(struct iphdr) + sizeof(struct tcphdr);
	strcpy(payload, "Payload message after both the headers");

	struct sockaddr_in sin, din;
	int one = 1;
	const int *val = &one;
	sd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
	perror("Raw-socket ");

	din.sin_family = AF_INET;
	din.sin_port = htons(9090);
	din.sin_addr.s_addr = inet_addr("127.0.0.2");

	iph->ihl = 5;
	iph->version = 4;
	iph->tos = 0;
	iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr) + strlen(payload);
	iph->id = htonl(54321);
	iph->frag_off = 0;
	iph->ttl = 255;
	iph->protocol = IPPROTO_TCP;
	iph->check = 0; // Checksum
	iph->saddr = inet_addr("127.0.0.1");
	iph->daddr = inet_addr("127.0.0.2");

	tcph->source = htons(8080);
	tcph->dest = htons(9090);
	tcph->seq = 0;
	tcph->ack_seq = 0;
	tcph->doff = 0;
	tcph->fin = 0;
	tcph->syn = 1;
	tcph->rst = 0;
	tcph->psh = 0;
	tcph->ack = 0;
	tcph->urg = 0;
	tcph->window = htonl(65535);
	tcph->check = 0;
	tcph->urg_ptr = 0;

	setsockopt(sd, IPPROTO_IP, IP_HDRINCL, val, sizeof(one));
	perror("setsockopt ");

	unsigned int count = sizeof(struct iphdr) + sizeof(struct tcphdr) + strlen(payload);
	sendto(sd, buffer, count, 0, (struct sockaddr *)&din, sizeof(din));
	perror("message sent ");

	close(sd);
	return 0;
}