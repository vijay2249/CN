//First I am sending data from P1 to P3 continously till NULL
//After termination I am sending data from P3 to P1 continously till NULL

#include<stdio.h>
#include<unistd.h>
#include <string.h>
int main(){
    int pfd1[2];
    int pfd2[2];

    pipe(pfd1);
    pipe(pfd2);

    char inputbuffer[100];
    char outputbuffer[100];

    int c=fork();
    if(c>0){
         close(pfd1[0]);close(pfd2[1]);
         while(1){//Read from P1 and send to P2
            
            memset(inputbuffer, 0, 100);
            printf("P1 is writing :\n");
            fgets(inputbuffer,sizeof(inputbuffer),stdin);
            write(pfd1[1],inputbuffer,sizeof(inputbuffer));
            if(strlen(inputbuffer)==1)break;
            sleep(1);
            
         }
               printf("-------------------------------From P3 to P1--------------------------------------\n");
          while(1){
            memset(outputbuffer, 0, 100);
            printf("P1 is reading from P2:\n");
            read(pfd2[0],outputbuffer,sizeof(outputbuffer));
            if(strlen(outputbuffer)==1)break;
            printf("%s\n",outputbuffer);
            sleep(1);
             
         }
         
    }
    else{
        dup2(0,20);
        dup2(1,21);

        dup2(pfd1[0],0);
        dup2(pfd2[1],1);

         char *args[]={"./P2",NULL};
         execvp(args[0],args);
    }
}