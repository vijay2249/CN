#include<stdio.h>
#include <string.h>
#include<unistd.h>
int main(){
  char inputbuffer[100];
  system("gcc p3.c -o p3");
  FILE * fp=popen("./p3","w");
  int fd=fileno(fp);
  dup2(fd,1);
  memset(inputbuffer,0,sizeof(inputbuffer));
  read(0,inputbuffer,sizeof(inputbuffer));
  write(1,inputbuffer,sizeof(inputbuffer));
}
