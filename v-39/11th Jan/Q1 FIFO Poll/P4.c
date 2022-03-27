#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
int main(){
    char inputbuffer[100];
    fgets(inputbuffer,sizeof(inputbuffer),stdin);
    int r=mkfifo("FIFO4",0666);
    int wffd=open("FIFO4",O_WRONLY);
    write (wffd,inputbuffer,sizeof(inputbuffer));
    close(wffd);
}