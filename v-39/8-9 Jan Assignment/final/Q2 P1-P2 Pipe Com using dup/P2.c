#include<stdio.h>
#include<unistd.h>
#include <string.h>
int main(){
    
    write(8,"I am Process P2\n",16);
    char outputbuffer[100];
    char inputbuffer[100];

    while(1){

        memset(outputbuffer, 0, 100);
        write(8,"P2 is reading from P1:\n",22);
        read(0,outputbuffer,sizeof(outputbuffer));
        if(strlen(outputbuffer)==1)break;
        write(8,outputbuffer,sizeof(outputbuffer));
        sleep(1);
    

        memset(inputbuffer, 0, 100);
        write(8,"P2 is writing :\n",16);
        read(8,inputbuffer,sizeof(inputbuffer));
        write(1,inputbuffer,sizeof(inputbuffer));
        if(strlen(inputbuffer)==1)break;
        sleep(1);
        
        
    }

}