#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
int main(){
    char outputbuffer[100];
    
    int rffd=open("GREEN",O_RDONLY);
    int r=read (rffd,outputbuffer,sizeof(outputbuffer));
    printf("%s",outputbuffer);
    close(rffd);
}