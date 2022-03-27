#include<stdio.h>
#include <string.h>
#include<unistd.h>
int main(){
      
      char inputbuffer[100];
      while(1){
            memset(inputbuffer,0,sizeof(inputbuffer));
            read(0,inputbuffer,sizeof(inputbuffer));
            printf("This is P3:\n");
            printf("%s",inputbuffer);
      }
      
}
