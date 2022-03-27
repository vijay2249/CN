#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

int main(){
  int pfd1[2], pfd2[2];
  if(pipe(pfd1) == -1 || pipe(pfd2) == -1){
    printf("Unable to create pipes.\n");
    exit(EXIT_FAILURE);
  }
  int c = fork();
  
}