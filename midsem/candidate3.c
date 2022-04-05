#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<string.h> 
#include<sys/types.h> 
#include<sys/socket.h> 
#include<arpa/inet.h> 
#include<netinet/in.h> 

#define PORT     8080 
#define MAXLINE  1024 

int main(){ 
  int sfd; 
  char buffer[MAXLINE]; 
  char hello[MAXLINE]; 
  struct sockaddr_in server; 
  
  // Creating socket file descriptor 
  if((sfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ){ 
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }
    // memset(&server, 0, sizeof(server));   

  // Filling server information
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = INADDR_ANY;
    
  int n, len;    

  // fgets(hello,1024,stdin);
  // sendto(sfd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *)&server, sizeof(server)); 
  // while(n = recvfrom(sfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &server, &len)){
  //   buffer[n] = '\0'; 
  //   printf("Result : %s\n", buffer); 
  // }
  printf("Displaying the response given to candidate 3 from DA, DG, DM..");
  while(1){
    // gets input whenever there is any data that is sent to this candidate 1
    bzero(buffer, MAXLINE);
    n = recvfrom(sfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &server, &len);
    buffer[n] = '\0';
    printf("Response from DA: %s", buffer);
  }
  
  close(sfd); 
  return 0; 
} 