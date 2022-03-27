#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<string.h>
int main(){
    char inputbuffer[100];
    
    int r=mkfifo("FIFO2",0666);
        int wffd=open("FIFO2",O_WRONLY);
         memset(inputbuffer, 0, 100);
        read(0,inputbuffer,sizeof(inputbuffer));
        write (wffd,inputbuffer,sizeof(inputbuffer));
        close(wffd);

    
}