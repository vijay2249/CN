#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<poll.h>
#include<string.h>

int main(){
  struct pollfd pnfds[3];
  mkfifo("FIFO2",0666);
  int rffd2=open("FIFO2",O_RDONLY);
  mkfifo("FIFO3",0666);
  int rffd3=open("FIFO3",O_RDONLY);
  mkfifo("FIFO4",0666);
  int rffd4=open("FIFO4",O_RDONLY);
  while(1){
    pnfds[0].fd=rffd2;
    pnfds[0].events=POLLIN;
    pnfds[1].fd=rffd3;
    pnfds[1].events=POLLIN;
    pnfds[2].fd=rffd4;
    pnfds[2].events=POLLIN;
    poll(pnfds,3,1);
    char buffer[100];
    if(pnfds[0].revents & POLLIN ){
      memset(buffer, 0, 100);
      int i=read(rffd2,buffer,100);
      // printf("Process P2 Data :");
      write(1,buffer,i);
    }
    if(pnfds[1].revents & POLLIN ){
      memset(buffer, 0, 100);
      int i=read(rffd3,buffer,100);
      // printf("Process P3 Data :");
      write(1,buffer,i);
    }
    if(pnfds[2].revents & POLLIN ){
      memset(buffer, 0, 100);
      int i=read(rffd4,buffer,100);
      // printf("Process P4 Data :");
      write(1,buffer,i);
    }
  }
}
