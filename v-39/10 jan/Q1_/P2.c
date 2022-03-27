#include<stdio.h>
#include <string.h>
#include<unistd.h>
int main(){
      
      
      FILE * fp=popen("./P3","w");
      int fd=fileno(fp);
     // dup2(fd,1);  
      
      char inputbuffer[100]; 
      while(1){
            memset(inputbuffer,0,sizeof(inputbuffer));
            read(0,inputbuffer,sizeof(inputbuffer));
            write(fd,inputbuffer,sizeof(inputbuffer));
      }
      
}
