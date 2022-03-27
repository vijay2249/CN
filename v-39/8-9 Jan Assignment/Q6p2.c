#include<stdio.h>
#include<unistd.h>
int main(){
   
   char buffer[20];
  fgets(buffer,sizeof(buffer),stdin);

  printf("%s :this is P2",buffer);

}