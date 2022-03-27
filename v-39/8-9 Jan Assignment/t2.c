#include<stdio.h>
#include<unistd.h>
int main(){
   
   int pfd1[2],pfd2[2];
   

   pipe(pfd1);
   pipe(pfd2);
   int c=fork();
   
   char buf1[20],buf2[20]; 
   char buf3[20],buf4[20]; 
   
   if(c>0){
       close(pfd1[0]);
       close(pfd2[1]);

         while(1){
             printf("ParentInput:");
             fgets(buf1,sizeof(buf1),stdin);
             write(pfd1[1],buf1,sizeof(buf1));
             
             read(pfd2[0],buf4,sizeof(buf4));
             if(buf4[0]==1)exit();
             printf("Child Output: %s",buf4);  
       
         }
   }
   else{
        close(pfd1[1]);
        close(pfd2[0]);
     
         while(1){
            read(pfd1[0],buf2,sizeof(buf2));
             if(buf2[0]==1)exit();
            printf("Parent Output: %s",buf2);  
             
            printf("Child Input:");
             fgets(buf3,sizeof(buf3),stdin);
             write(pfd2[1],buf3,sizeof(buf3));
        }
      
   }

}