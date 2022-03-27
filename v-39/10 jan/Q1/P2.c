#include<stdio.h>
#include <string.h>
#include<unistd.h>
int main(){
      
    //   printf("This is P2\n");
      char inputbuffer[100];
      FILE * fp=popen("./P3","w");
      int fd=fileno(fp);
      dup2(fd,1);
      while(1){
            memset(inputbuffer,0,sizeof(inputbuffer));
            read(0,inputbuffer,sizeof(inputbuffer));
            write(1,inputbuffer,sizeof(inputbuffer));
      }
      
}
