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
    
    //creating rsfd
    int rsfd=socket(AF_INET,SOCK_RAW,2);
    perror("creating socket");

    //targeting client
    struct sockaddr_in client;
    client.sin_family=AF_INET;
    client.sin_addr.s_addr=INADDR_ANY;

    printf("Enter content to send to client\n");
    char buf[1024];
    scanf("%[^\n]%*c",buf);

    sendto(rsfd,buf,sizeof(buf),0,(struct sockaddr*)&client,sizeof(client));
    perror("sending");

    return 0;
}