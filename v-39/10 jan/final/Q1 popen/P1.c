#include<stdio.h>
#include<unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include<string.h>
#include<fcntl.h>



void *send_thread(void *arg){
    int *ptr;
    int pfd1;
    ptr = (int *) arg;
    pfd1 = *ptr;

    char inputBuffer1[100];
            memset(inputBuffer1, 0, 100);
            printf("\nThis is P1 Writing to P2:");   
            fgets(inputBuffer1,sizeof(inputBuffer1),stdin);

            if(strlen(inputBuffer1)==1)exit(0);
            write(pfd1,inputBuffer1,sizeof(inputBuffer1));
}
void *recieve_thread(void *arg){
    int *ptr;
    int pfd0;
    ptr = (int *) arg;
    pfd0 = *ptr;
    char outputBuffer1[100]; 
            memset(outputBuffer1, 0, 100);
            read(pfd0,outputBuffer1,sizeof(outputBuffer1));
            printf("Green FIFO Output inside P1: %s\n",outputBuffer1);  
}
int main(){

      FILE * fp=popen("./P2","w");
      char inputbuffer[100];
      int fd=fileno(fp);
      
             pthread_t t1;
             pthread_create(&t1,NULL,send_thread,(void *)&fd);
             pthread_join(t1,NULL);  

          
            int rffd=open("GREEN",O_RDONLY);
            pthread_t t2;
            pthread_create(&t2,NULL,recieve_thread,(void *)&rffd);
            pthread_join(t2,NULL); 

 

    
}
