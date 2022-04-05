// // this file contains all the details about the candidate
// // request candidates for the information about them
// // save them in buffer, when requested send it to A,G,M

// #include<stdlib.h>
// #include<stdio.h>
// #include<string.h>
// #include<unistd.h>
// #include<sys/types.h>
// #include<sys/socket.h>
// #include<netinet/in.h>
// #include<netdb.h>

// int n1,n2,n3;

// void error(const char *msg){
//   perror(msg);
//   exit(1);
// }

// #define PORT 4444

// // file hostname  - ex: ./client localhost
// int main(int argc, char *argv[]){
//   struct sockaddr_in server;
//   struct hostent *s;
//   struct sockaddr_in server, A, G, M, DA, DG, DM;
//   socklen_t Alen, Glen, Mlen, DAlen, DGlen, DMlen;
//   char bufferC1[1024],bufferC2[1024],bufferC3[1024];
//   int sockfd = socket(AF_INET, SOCK_STREAM, 0);
//   if(sockfd < 0) {error("socket error");}

//   s = gethostbyname(argv[1]);
//   if(s == NULL) { fprintf(stderr, "error, no host found with the IP address");}

//   server.sin_family = AF_INET;
//   bcopy((char *) s->h_addr, (char *)&server.sin_addr.s_addr, s->h_length);
//   server.sin_port = htons(PORT);
//   if(connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0){
//     error("connection failed");
//   }
  
//   // requesting info about candidates
//   bzero(bufferC1, 1024);
//   bzero(bufferC2, 1024);
//   bzero(bufferC3, 1024);
//   n1 = read(sockfd, bufferC1, 1024);
//   n2 = read(sockfd, bufferC2, 1024);
//   n3 = read(sockfd, bufferC3, 1024);
//   if(n1 < 0 || n2 < 0 || n3 < 0){ error("read error");}
//   printf("Data about C1: %s", bufferC1);
//   printf("Data about C2: %s", bufferC2);
//   printf("Data about C3: %s", bufferC3);
//   bzero(bufferC1, 1024);

//   close(sockfd);
//   return 0;
// }


// request data from candidates
// send data to A,G,M

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

#define R_PORT 8888
#define CANDIDATE_PORT 5555

int main(int argc, char *argv[]){
  char bufferC1[1024],bufferC2[1024],bufferC3[1024];
  
  struct sockaddr_in server, A, G, M, C1, C2, C3;
  socklen_t Alen, Glen, Mlen, C1len, C2len, C3len;
  
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0){error("error opening socket");}

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(CANDIDATE_PORT);

  if(bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0){ error("bind error");}

  // number of clients to listen to - in this case clients are A,G,M
  listen(sockfd, 6);
  Alen = sizeof(A);
  Glen = sizeof(G);
  Mlen = sizeof(M);
  C1len = sizeof(C1);
  C2len = sizeof(C2);
  C3len = sizeof(C3);

  // ------- part one  -> reading data from the candidates C1, C2, C3

  int new_c1_sockfd = accept(sockfd, (struct sockaddr *)&C1, &C1len);
  int new_c2_sockfd = accept(sockfd, (struct sockaddr *)&C2, &C2len);
  int new_c3_sockfd = accept(sockfd, (struct sockaddr *)&C3, &C3len);
  if(new_c1_sockfd <0 || new_c2_sockfd<0 || new_c3_sockfd<0){ perror("accept error,");exit(EXIT_FAILURE);}
  
  // clear all the buffer data that is if already present in the variable
  bzero(bufferC1, 1024);
  bzero(bufferC2, 1024);
  bzero(bufferC3, 1024);

  // read the data from the clients A,G,M
  int c1 = read(new_c1_sockfd, bufferC1, 1024);
  int c2 = read(new_c2_sockfd, bufferC2, 1024);
  int c3 = read(new_c3_sockfd, bufferC3, 1024);

  if(c1 <0 || c2< 0 || c3 < 0) {perror("Error in reading data"); exit(EXIT_FAILURE);}

  // print the data sent by the clients A,G,M
  printf("Candidate-1 info: %s", bufferC1);
  printf("Candidate-2 info: %s", bufferC2);
  printf("Candidate-3 info: %s", bufferC3);
  printf("Reading data from the candidates is done.....\n");


  // --------  part -2 -> writing the data to the clients A, M, G
  printf("Sendind data to the clients A, G, M..... starts...\n");

  int new_a_sockfd = accept(sockfd, (struct sockaddr *)&A, &Alen);
  int new_g_sockfd = accept(sockfd, (struct sockaddr *)&G, &Glen);
  int new_m_sockfd = accept(sockfd, (struct sockaddr *)&M, &Mlen);

  if(new_a_sockfd <0 || new_g_sockfd< 0 || new_m_sockfd < 0) {perror("Error in reading data"); exit(EXIT_FAILURE);}

  while(1){
    char b[2];
    bzero(b,2);
    int n = read(new_a_sockfd, b,2);
    if(n<0){perror("read error"); exit(EXIT_FAILURE);}
    n = write(new_a_sockfd, bufferC1, 1024);
  }

  // send the data sent by candidates to the clients A,G,M
  int a = write(new_a_sockfd, bufferC1, 1024);
  int g = write(new_g_sockfd, bufferC2, 1024);
  int m = write(new_m_sockfd, bufferC3, 1024);

  if( a<0 || g<0 || m<0) { perror("Writing error"); exit(EXIT_FAILURE);}

  close(new_c1_sockfd);
  close(new_c2_sockfd);
  close(new_c3_sockfd);
  close(new_a_sockfd);
  close(new_g_sockfd);
  close(new_m_sockfd);
  close(sockfd);
  
  return 0;
}