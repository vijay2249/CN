#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>


#include <sys/wait.h>
int main(){
    int pid;
    printf("%s\n","This is Main...........1");
    pid=fork();
    if(pid>0){
        printf("%s\n","This is Parent...........2");
         wait(NULL);
        printf("%s\n","This is Child Terminated...........4");
    }
    else{
         printf("%s\n","This is Child...........3");
    }

}
