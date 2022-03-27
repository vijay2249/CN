#include<stdio.h>
#include<unistd.h>
#include <string.h>  
#include <stdlib.h>
#include <pthread.h>
void *send_thread(void *arg){
    int *ptr;
    int pfd1;
    ptr = (int *) arg;
    pfd1 = *ptr;

    char inputBuffer1[100];
            memset(inputBuffer1, 0, 100);
            printf("\nParent Pipe Input inside Parent:");   
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
            printf("Parent Pipe Output inside Child: %s",outputBuffer1);  
}
int main(){
   
   int pfd1[2];

   pipe(pfd1);

   int pid=fork();
   
   if(pid>0){
       close(pfd1[0]);

         while(1){
          pthread_t t1;
          pthread_create(&t1,NULL,send_thread,(void *)&pfd1[1]);
          pthread_join(t1,NULL);  
       }
   }
   else{
        close(pfd1[1]);

       while(1){ 
            pthread_t t2;
          pthread_create(&t2,NULL,recieve_thread,(void *)&pfd1[0]);
          pthread_join(t2,NULL);  
           }
      
   }

}