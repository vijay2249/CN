#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h> 
#include <string.h>
#include <unistd.h>
int main(){

    char  buffer[100];

    read(0,buffer,sizeof(buffer)); //reading input in form of char array
    
    
    //dividing the string into numbers
    int temp[6],k=0;
    char *f1num = strtok(buffer," \n"); 
    while(f1num != NULL){
        temp[k++]=atoi(f1num);
        f1num=strtok(NULL," \n");
    }

    //Perform the operations and printing
    int sum=temp[0]+temp[1];
    printf("%d\n",sum);

    int diff=temp[2]-temp[3];
    printf("%d\n",diff);

    int mult=temp[4]*temp[5];
    printf("%d\n",mult);
}