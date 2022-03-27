#include<iostream>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

using namespace std;

int main(){

  mkfifo("send_data", 0666);
  mkfifo("receive_data", 0666);
  while(1){
    int receive_fd = open("receive_data", O_RDONLY);
    cout<<"SERVER FIFO opened\n";
    char c, msg[2][1024];
    int j = 0, d = 0;
    while(read(receive_fd, &c, 1)){
      if(c == '$'){
        msg[d][j++] = '\0';
        j=0;d++;
      }
      else msg[d][j++] = c;
    }
    int send_fd = open("send_data", O_WRONLY);

    write(send_fd, msg[0], 1024);
    read(receive_fd, msg[1], 1024);
    close(send_fd);
    close(receive_fd);
  }
  return 0;
}