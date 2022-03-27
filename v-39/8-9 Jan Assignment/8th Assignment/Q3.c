#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main(){
    int pfd1[2];
    pipe(pfd1);
    
    int c1=fork();
    if(c1>0){//P1 parent
      wait(&c1);
    }
    else{//P1 Child
           char inputbuffer[100];
           printf("I am Process P1 : Enter Input:");
           fgets(inputbuffer,sizeof(inputbuffer),stdin); 
           write(pfd1[1],inputbuffer,sizeof(inputbuffer));
           
           dup2(0,4);
           dup2(pfd1[0],0);

          char *args[]={"./Q3P2",NULL};
          execvp(args[0],args);
    }

}

