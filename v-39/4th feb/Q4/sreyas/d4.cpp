#include<iostream>
#include <unistd.h>
#include<stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include<arpa/inet.h>
#include<poll.h>
using namespace std;
int main()
{
    char a[1000];
    for(int i=0;i<1000;i++)
    a[i]=0;
    while(1){
    read(0,a,1000);
    int i;
    for(i=0;i<1000 && a[i]!='\0';i++)
    if(a[i]=='\0')
    break;
    write(1,a,i);
    for(i=0;i<1000;i++)
    a[i]=0;
}
}