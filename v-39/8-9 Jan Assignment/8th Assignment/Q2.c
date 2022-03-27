#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h> 
#include<unistd.h>
#include<sys/wait.h>
int main(){
   
    char inputbuffer[20];
    // fgets(inputbuffer,sizeof(inputbuffer),stdin); 
    int pfd1[2];
    pipe(pfd1);
    int c=fork();

    if(c>0){
        sleep(2);
           wait(&c);
            char buffer[100];
             printf("I am Process P1 \n");
             
            int n=read(0,buffer,sizeof(buffer));
            printf("Output :%s",buffer);

        //    dup2(7,0);


    }
    else{
           printf("I am Process P1 : Enter Input:");
           fgets(inputbuffer,sizeof(inputbuffer),stdin); 
           write(pfd1[1],inputbuffer,sizeof(inputbuffer));
           
           dup2(0,4);
           dup2(pfd1[0],0);
          char *args[]={"./Q2P2",NULL};
          execvp(args[0],args);
             
          
    }
   
}