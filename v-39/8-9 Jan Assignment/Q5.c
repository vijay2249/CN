#include<stdio.h>
#include<unistd.h>
int main(){
   
   int pfd1[2],pfd2[2];
   char inputBuffer1[20],outputBuffer1[20]; 
   char inputBuffer2[20],outputBuffer2[20]; 

   pipe(pfd1);
   pipe(pfd2);
   int pid=fork();
   
   if(pid>0){
       close(pfd1[0]);
       close(pfd2[1]);

         while(1){
             printf("Parent Pipe Input:");
             fgets(inputBuffer1,sizeof(inputBuffer1),stdin);
             write(pfd1[1],inputBuffer1,sizeof(inputBuffer1));
             
             read(pfd2[0],outputBuffer2,sizeof(outputBuffer2));
             printf("Child Pipe Output inside Parent: %s",outputBuffer2);  
       
         }
   }
   else{
        close(pfd1[1]);
        close(pfd2[0]);
     
         while(1){
            read(pfd1[0],outputBuffer1,sizeof(outputBuffer1));
            printf("Parent Pipe Output inside Child: %s",outputBuffer1);  
             
            printf("Child Pipe Input:");
             fgets(inputBuffer2,sizeof(inputBuffer2),stdin);
             write(pfd2[1],inputBuffer2,sizeof(inputBuffer2));
        }
      
   }

}