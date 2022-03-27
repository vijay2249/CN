
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 

#include<string.h>

char buffer[1024]; 

void numberize(){
     for(int i=0;buffer[i]!='\0';i++){
          if(buffer[i]>='a' && buffer[i]<='z'){
              buffer[i]='$';
          }
     }
}
int main(){
    
     memset(buffer,'\0',sizeof(buffer));
	 read(0,buffer,sizeof(buffer));
     printf("message recieved by S3 : %s",buffer);

     numberize();

	 write(1,buffer,sizeof(buffer));
	 	
}