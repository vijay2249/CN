// this file receives data from the server file using raw sockets
// data is sent from A,G,M is received by server and will be send to clients da, dg, dm

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

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

  bzero(buffer, 1024);
  int n = read(sockfd, buffer, 1024);
  if(n < 0){ error("read error");}
  printf("Data from M: %s", buffer);
  bzero(buffer, 1024);

  close(sockfd);
  return 0;
}
