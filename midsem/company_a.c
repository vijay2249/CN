#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

int n;

void error(const char *msg){
  perror(msg);
  exit(1);
}

#define R_PORT 8888
#define PORT 4444

// file hostname  - ex: ./client localhost
int main(int argc, char *argv[]){
  struct sockaddr_in server;
  struct hostent *s;
  char buffer[1024];
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {error("socket error");}

  // connect to the local network for sending data to DA, DG, DM
  s = gethostbyname(argv[1]);
  if(s == NULL) { fprintf(stderr, "error, no host found");}

  server.sin_family = AF_INET;
  bcopy((char *) s->h_addr, (char *)&server.sin_addr.s_addr, s->h_length);
  server.sin_port = htons(R_PORT);

  // this connects with the main server code file rather than to receiver.c file sockfd
  if(connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0){
    error("connection failed");
  }

  // request candidate data from the process(server) that contains all candidate details
  while(1){
    bzero(buffer, 1024);
    int r_socket;
    if((r_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){perror("socket error"); exit(EXIT_FAILURE);};
    printf("Enter which cadidate data you need: ");
    fgets(buffer, 1024, stdin);
    if(strcmp(buffer, "exit") == 0){break;}
    n = write(r_socket, buffer, strlen(buffer));
    if(n<0) {perror("write error");}
    n = read(r_socket, buffer, 1024);
  }

  

  // send data to respective display board
  bzero(buffer, 1024);
  fgets(buffer, 1024, stdin);
  n = write(sockfd, buffer, strlen(buffer));
  if(n < 0){ error("write error");}
  bzero(buffer, 1024);

  close(sockfd);
  return 0;
}
