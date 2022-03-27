#include<stdio.h>
#include<unistd.h>
#include <string.h>
int main(){

    //write(21,"I am Process P2\n",16);

    int pfd3[2];
    int pfd4[2];

    pipe(pfd3);
    pipe(pfd4);

    char PassingBuffer1[100];
    char PassingBuffer2[100];

    int c=fork();
    if(c>0){
       
         while(1){//Read from P1 and send to P2
            
            memset(PassingBuffer1, 0, 100);
             write(21,"P2 is sending to P3\n",20);
             read(0,PassingBuffer1,sizeof(PassingBuffer1));
            write(pfd3[1],PassingBuffer1,sizeof(PassingBuffer1));
            if(strlen(PassingBuffer1)==1)break;
            sleep(1);
            
         }
         while(1){
             memset(PassingBuffer2, 0, 100);
              write(21,"P2 is sending from P3 to P1\n",28);
            read(pfd4[0],PassingBuffer2,sizeof(PassingBuffer2));
            write(1,PassingBuffer2,sizeof(PassingBuffer2));
           if(strlen(PassingBuffer2)==1)break;
            sleep(1);
             
         }
         
    }
    else{
 
        dup2(pfd3[0],0);
        dup2(pfd4[1],1);

         char *args[]={"./P3",NULL};
         execvp(args[0],args);
    }
}