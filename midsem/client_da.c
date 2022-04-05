// data from the company A will be sent here and we need to display the data

#include<stdio.h>
#include<netdb.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h> 

void error(const char *msg){
  perror(msg);
  exit(1);
}

#define PORT 4444
#define UDPPORT 8080

// file hostname  - ex: ./client localhost
int main(int argc, char *argv[]){
  struct sockaddr_in server;
  struct hostent *s;
  char buffer[1024];
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {error("socket error");}

  s = gethostbyname(argv[1]);
  if(s == NULL) { fprintf(stderr, "error, no host found with the IP address");}

  server.sin_family = AF_INET;
  bcopy((char *) s->h_addr, (char *)&server.sin_addr.s_addr, s->h_length);
  server.sin_port = htons(PORT);
  if(connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0){
    error("connection failed");
  }

  // display the data sent by company A

  bzero(buffer, 1024);
  int n = read(sockfd, buffer, 1024);
  if(n < 0){ error("read error");}
  printf("Data from A: %s", buffer);
  bzero(buffer, 1024);

  // we need to send this data from company A to all candidates using UDP
  // first connect to multiple candidates and then send the data from respective company -> now this one acts as server with multiple clients

  struct sockaddr_in s,c1,c2,c3;
  int sfd1, sfd2, sfd3;
  if((sfd1 = socket(AF_INET, SOCK_DGRAM, 0))< 0){perror("C1 socket failure"); exit(EXIT_FAILURE);}
  if((sfd2 = socket(AF_INET, SOCK_DGRAM, 0))< 0){perror("C2 socket failure"); exit(EXIT_FAILURE);}
  if((sfd3 = socket(AF_INET, SOCK_DGRAM, 0))< 0){perror("C3 socket failure"); exit(EXIT_FAILURE);}

  if(bind(sfd1, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ){
    perror("bind with C1 failed");
    exit(EXIT_FAILURE);
  }
  if(bind(sfd2, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ){
    perror("bind with C2 failed");
    exit(EXIT_FAILURE);
  }
  if(bind(sfd3, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ){
    perror("bind with C3 failed");
    exit(EXIT_FAILURE);
  }

  // int len = sizeof(c1);
  sendto(sfd1, (const char *)buffer, strlen(buffer), MSG_CONFIRM, 
          (const struct sockaddr *)&c1, sizeof(c1));
  
  sendto(sfd2, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&c2, sizeof(c2));
  sendto(sfd3, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&c3, sizeof(c3));


  close(sockfd);
  return 0;
}
