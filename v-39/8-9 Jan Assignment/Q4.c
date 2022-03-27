#include<stdio.h>
#include<unistd.h>
int main(){
   
   int pfd[2];
   char inputBuffer[20],buffer[20]; 
   
   pipe(pfd);
   int pid=fork();
   
   if(pid>0){
       close(pfd[0]);
       while(1){

           printf("Pipe Input:");
            fgets(inputBuffer,sizeof(inputBuffer),stdin);
             write(pfd[1],inputBuffer,sizeof(inputBuffer));
             sleep(1);
       }
     

   }
   else{
        close(pfd[1]);
       while(1){
            read(pfd[0],buffer,sizeof(buffer));
            printf("Pipe Output: %s",buffer);  
             sleep(1);
       }
      
   }

}