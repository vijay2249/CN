#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<time.h>

#define len 1024
#define port1 4444

int main(int argc, char *argv[]){
  struct sockaddr_in server1;
  char buffer1[len];

  int sfd = socket(AF_INET, SOCK_STREAM, 0);

  memset(buffer1, '0', sizeof(buffer1));
  memset(&server1, '0', sizeof(server1));

  server1.sin_family = AF_INET;
  server1.sin_port = htons(port1);
  server1.sin_addr.s_addr = htonl(INADDR_ANY);

  bind(sfd, (struct sockaddr*)&server1, sizeof(server1));

  listen(sfd, 10);
  while(1){
    int cfd = accept(sfd, (struct sockaddr *)NULL, NULL);
  }

  return 0;
}
