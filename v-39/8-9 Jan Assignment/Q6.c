#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h> 
#include<unistd.h>
#include<sys/wait.h>
int main(){
   
    char inputbuffer[20];
    fgets(inputbuffer,sizeof(inputbuffer),stdin); 
    dup2(1,3);
    int fd=open("./Q6p2",O_WRONLY);
    dup2(fd,1);
    
    int pid=fork();
    if(pid>0){
        wait(NULL);
    }  
    else{
        printf("%s",inputbuffer);
        char *args[]={"./Q6p2",NULL};
          execvp(args[0],args);
    }
    dup2(3,1);
}