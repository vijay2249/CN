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
        int pid2=fork();
        if(pid2>0){
             wait(NULL);
             printf("P3 is Terminated\n");
        }
        else{
            char *args[]={"./P3",NULL};
            execvp(args[0],args);
            
        }
        
         wait(NULL);
         printf("P2 is Terminated\n");
        printf("%s\n","This is Child Terminated...........4");
    }
    else{
         printf("%s\n","This is Child...........3");
          char *args[]={"./P2",NULL};
          execvp(args[0],args);
    }
   
}

