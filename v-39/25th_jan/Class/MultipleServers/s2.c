
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 

#include<string.h>

char buffer[1024]; 

void smallize(){
     for(int i=0;buffer[i]!='\0';i++){
          if(buffer[i]>='A' && buffer[i]<='Z'){
              buffer[i]=buffer[i]+32;
          }
     }
}
int main(){
    
     memset(buffer,'\0',sizeof(buffer));
	 read(0,buffer,sizeof(buffer));
     printf("message recieved by S2 : %s",buffer);

     smallize();

	 write(1,buffer,sizeof(buffer));
	 	
}