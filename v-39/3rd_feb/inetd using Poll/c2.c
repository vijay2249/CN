#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
   
#define PORT     7070
#define MAXLINE 1024
   
// Driver code
int main() {
    int sfd;
    char buffer[MAXLINE];
    char *hello = "Hello from client";
    struct sockaddr_in  addr;
   

    if((sfd = socket(AF_INET, SOCK_DGRAM, 0))==-1){
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
   
    memset(&addr, 0, sizeof(addr));
       
    // Filling server information
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
       
    int n, len;
       
    sendto(sfd, (const char *)hello, strlen(hello),
            MSG_CONFIRM, (const struct sockaddr *) &addr, sizeof(addr));
    printf("Hello message sent.\n");
           
    n = recvfrom(sfd, (char *)buffer, MAXLINE, 
            MSG_WAITALL, (struct sockaddr *) &addr,&len);
    buffer[n] = '\0';
    printf("Server : %s\n", buffer);
   
    close(sfd);
    return 0;
}