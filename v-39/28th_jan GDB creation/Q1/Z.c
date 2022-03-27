#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h> 
#include <string.h>
#include <unistd.h>
int main(){

     

    int f1=open("IT.txt",O_RDONLY);
    if (f1==-1){perror("file IT can't be opened"); exit(1);} 
    char c1[100];
    int s1=read(f1,c1,sizeof(c1));
    c1[s1]='\0';

    int temp[6],k=0;
    char *f1num = strtok(c1," \n");
    while(f1num != NULL){
        temp[k++]=atoi(f1num);
        f1num=strtok(NULL," \n");
    }
     
    int f2=open("OT.txt" ,O_WRONLY | O_CREAT);

    char buf[3];
    sprintf(buf, "%d\t", temp[0]+temp[1]);
    write(f2,buf,sizeof(buf));

    sprintf(buf, "%d\t", temp[2]-temp[3]);
    write(f2,buf,sizeof(buf));

    sprintf(buf, "%d\t", temp[4]*temp[5]);
    write(f2,buf,sizeof(buf));
    

}