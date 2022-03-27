#include<stdio.h>
#include<unistd.h>
int main(){

      FILE * fp=popen("./P2","w");
      char inputbuffer[100];
      int fd=fileno(fp);
      while(1){
            sleep(1);
             printf("This is P1 Writing to P2:\n");
            fgets(inputbuffer,sizeof(inputbuffer),stdin);
            write(fd,inputbuffer,sizeof(inputbuffer));
            sleep(1);
      }
     
      
}