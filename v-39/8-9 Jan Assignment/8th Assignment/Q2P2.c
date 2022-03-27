#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h> 
#include<unistd.h>
#include<sys/wait.h>
int main(){
     printf("I am Process P2 \n");
    char buffer[100];
     char inputbuffer[20];
    int n=read(0,buffer,sizeof(buffer));
    printf("Output :%s",buffer);

    int pfd2[2];
    pipe(pfd2);
    
     dup2(4,0);
         printf("Inside Process P2 : Enter Input:");

         
         fgets(inputbuffer,sizeof(inputbuffer),stdin); 
         printf("--%s--",inputbuffer);
         write(pfd2[1],inputbuffer,sizeof(inputbuffer));
      // dup2(0,4);
         dup2(pfd2[0],0);

      

    // int c=fork();
    // if(c>0){
    //         wait(&c);
    // }
    // else{
       
        //  char *args[]={"./Q2",NULL};
        //  execvp(args[0],args);

    // }
}
