#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main(){
    
    printf("This is Process P2");
    
    int c2=fork();
    if(c2>0){//P2 parent
      wait(&c2);
      
    }
    else{//P2 Child
          char *args[]={"./Q3P3",NULL};
          execvp(args[0],args);
    }

}
