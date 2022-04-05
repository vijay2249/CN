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
  char bufferA[1024],bufferG[1024],bufferM[1024];
  
  struct sockaddr_in server, A, G, M, DA, DG, DM;
  socklen_t Alen, Glen, Mlen, DAlen, DGlen, DMlen;
  
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0){error("error opening socket");}

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(PORT);

  if(bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0){ error("bind error");}

  // number of clients to listen to - in this case clients are A,G,M
  listen(sockfd, 6);
  Alen = sizeof(A);
  Glen = sizeof(G);
  Mlen = sizeof(M);
  DAlen = sizeof(DA);
  DGlen = sizeof(DG);
  DMlen = sizeof(DM);

  // ------- part one  -> reading data from the clients A,G,M

  int new_a_sockfd = accept(sockfd, (struct sockaddr *)&A, &Alen);
  int new_g_sockfd = accept(sockfd, (struct sockaddr *)&G, &Glen);
  int new_m_sockfd = accept(sockfd, (struct sockaddr *)&M, &Mlen);
  printf("new_a_socket: %d\nnew_g_socket: %d\nnew_m_socket: %d\n", new_a_sockfd, new_g_sockfd, new_m_sockfd);

  if(new_a_sockfd < 0){ error("accept error"); }
  if(new_g_sockfd < 0){ error("accept error"); }
  if(new_m_sockfd < 0){ error("accept error"); }
  
  // clear all the buffer data that is if already present in the variable
  bzero(bufferA, 1024);
  bzero(bufferG, 1024);
  bzero(bufferM, 1024);

  // read the data from the clients A,G,M
  int a = read(new_a_sockfd, bufferA, 1024);
  int g = read(new_g_sockfd, bufferG, 1024);
  int m = read(new_m_sockfd, bufferM, 1024);

  if(a < 0){ error("error reading from A data"); }
  if(g < 0){ error("error reading from G data"); }
  if(m < 0){ error("error reading from M data"); }

  // print the data sent by the clients A,G,M
  printf("Client A: %s", bufferA);
  printf("Client G: %s", bufferG);
  printf("Client M: %s", bufferM);
  printf("Reading data from the clients A,G,M is done.....\n");


  // --------  part -2 -> writing the data to the clients DA, DM, DG
  printf("Writing data to the clients DA, DG, DM..... starts...\n");

  int new_da_sockfd = accept(sockfd, (struct sockaddr *)&DA, &DAlen);
  int new_dg_sockfd = accept(sockfd, (struct sockaddr *)&DG, &DGlen);
  int new_dm_sockfd = accept(sockfd, (struct sockaddr *)&DM, &DMlen);

  if(new_da_sockfd < 0){ error("accept error"); }
  if(new_dg_sockfd < 0){ error("accept error"); }
  if(new_dm_sockfd < 0){ error("accept error"); }

  // send the data sent by A,G,M to the clients DA,DG,DM
  int da = write(new_da_sockfd, bufferA, 1024);
  int dg = write(new_dg_sockfd, bufferG, 1024);
  int dm = write(new_dm_sockfd, bufferM, 1024);

  if(da < 0){ error("error writing to DA data"); }
  if(dg < 0){ error("error writing to DG data"); }
  if(dm < 0){ error("error writing to DM data"); }

  close(new_a_sockfd);
  close(new_g_sockfd);
  close(new_m_sockfd);
  close(new_da_sockfd);
  close(new_dg_sockfd);
  close(new_dm_sockfd);
  close(sockfd);
  
  return 0;
}