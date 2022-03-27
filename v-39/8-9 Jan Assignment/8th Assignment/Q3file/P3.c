#include<stdio.h>
#include<unistd.h>
#include <string.h>
int main(){
    
    write(21,"I am Process P3\n",16);
    char outputbuffer[100];
    char inputbuffer[100];

    while(1){

        memset(outputbuffer, 0, 100);
        write(21,"P3 is reading from P2:\n",23);
        read(0,outputbuffer,sizeof(outputbuffer));
        if(strlen(outputbuffer)==1)break;
        write(21,outputbuffer,sizeof(outputbuffer));
        write(21,"\n",1);
        sleep(1);
    
        
    }
    while(1){
         memset(inputbuffer, 0, 100);
        write(21,"P3 is writing :\n",16);
        read(20,inputbuffer,sizeof(inputbuffer));
        write(1,inputbuffer,sizeof(inputbuffer));
        if(strlen(inputbuffer)==1)break;
        sleep(1);
        
    }

}