#include<stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include <pthread.h>
#include<string.h>
void *sending_thread(void *arg){
    int *ptr;
    int pfd1;
    ptr = (int *) arg;
    pfd1 = *ptr;

    char inputBuffer1[100];
            memset(inputBuffer1, 0, 100);
            printf("\nClient Sending msg:");   
            fgets(inputBuffer1,sizeof(inputBuffer1),stdin);

            if(strlen(inputBuffer1)==1)exit(0);
            write(pfd1,inputBuffer1,sizeof(inputBuffer1));
}
void *recieveing_thread(void *arg){
    int *ptr;
    int pfd0;
    ptr = (int *) arg;
    pfd0 = *ptr;
    char outputBuffer1[100]; 
            memset(outputBuffer1, 0, 100);
            read(pfd0,outputBuffer1,sizeof(outputBuffer1));
            printf("Client Recieving msg: %s",outputBuffer1);  
}

int main(){
    char outputbuffer[100];
    
    int rffd=open("GREEN",O_RDONLY);

    pthread_t t1;
          pthread_create(&t1,NULL,recieveing_thread,(void *)&rffd);
          pthread_join(t1,NULL);  

    
   
}