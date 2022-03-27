#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

void error(const char *msg){
  perror(msg);
  exit(1);
}

#define PORT 4444

int main(int argc, char *argv[]){
  // int sockfd, newsockfd, n;
  char buffer[1024];
  
  struct sockaddr_in server, c1;
  socklen_t c1len;
  
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0){error("error opening socket");}

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(PORT);

  if(bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0){ error("bind error");}

  listen(sockfd, 5);
  c1len = sizeof(c1);

  int newsockfd = accept(sockfd, (struct sockaddr *)&c1, &c1len);
  if(newsockfd < 0){ error("accept error"); }
  
  while(1){
    bzero(buffer, 1024);
    int n = read(newsockfd, buffer, 1024);
    if(n < 0){ error("read fail"); }
    printf("Client: %s", buffer);
    bzero(buffer, 1024);
    fgets(buffer, 1024, stdin);
    n = write(newsockfd, buffer, strlen(buffer));
    if(n < 0){ error("write fail"); }
    if(strcmp("Bye", buffer) == 0){ break; }
  }
  close(newsockfd);
  close(sockfd);
  return 0;
}