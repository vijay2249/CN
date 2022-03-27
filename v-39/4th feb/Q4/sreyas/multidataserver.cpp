#include <unistd.h>
#include<stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include<arpa/inet.h>
#include<poll.h>
#define PORT 8080
using namespace std;
int main()
{
    int sfd,nsfd[100],p,d[100],c[4],fd[8][2];
    for(int i=0;i<4;i++)
    c[i]=-1;
    for(int i=0;i<100;i++)
    d[i]=1;
    for(int i=0;i<100;i++)
    nsfd[i]=-1;
    struct sockaddr_in address;
    socklen_t add=sizeof(address);
    struct sockaddr_in naddress[5];
    socklen_t nadd[5];
    char buffer[1000];
    sfd=socket(AF_INET, SOCK_STREAM, 0);
    int opt=1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt));
    address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons( PORT );
    bind(sfd,(struct sockaddr *)&address,sizeof(address));
    listen(sfd,100);
    int i=0;
    struct pollfd pfd[100];
    for(int i=0;i<100;i++)
    pfd[i].fd=-1;
    pfd[0].fd=sfd;
    pfd[0].events=POLLIN;
    while(1)
    {
        cout<<"entered to while loop"<<endl;
        for(int j=1;j<=i;j++)
        {
            pfd[j].fd=nsfd[j-1];
            pfd[j].events=POLLIN;
        }
        poll(pfd,i+1,-1);
        int k=i;
        int flag=0;
        if(pfd[0].revents && POLLIN)
        {
            cout<<"coming here"<<endl;
            while(nsfd[i]==-1)
            {
                nsfd[i]=accept(sfd,(struct sockaddr *)&naddress[i],&nadd[i]);
                cout<<"value of nsfd is"<<nsfd[i]<<endl;
            }
            i++;
            flag=1;
        }
        for(int j=1;j<=i && flag==0;j++)
        {
            cout<<j<<"val of j "<<endl;
            if(pfd[j].revents && POLLIN)
            {
                k=j-1;
                break;
            }
        }
        cout<<k<<" "<<i<<" val of k and i"<<endl;
        if(i!=0)
        if(k!=i)
        {
            cout<<"ready to recive"<<endl;
            recv(nsfd[k],buffer,sizeof(buffer),0);
            cout<<buffer<<endl;
            if(int(buffer[0])>=49 && int(buffer[0])<=52 )
            {
                d[k]=int(buffer[0])-48;
                cout<<"entering here" <<d[k]<<endl;
                recv(nsfd[k],buffer,sizeof(buffer),0);
            }
            p=d[k];
            if(p==1)
            {
                if(c[0]==-1)
                {
                    pipe(fd[0]);
                    pipe(fd[1]);
                    c[0]=fork();
                }
                if(c[0]>0)
                {
                    write(fd[1][1],buffer,sizeof(buffer));
                    read(fd[0][0],buffer,sizeof(buffer));
                    send(nsfd[k],buffer,sizeof(buffer),0);
                }
                else if(c[0]==0)
                {
                    dup2(fd[1][0],0);
                    dup2(fd[0][1],1);
                    char *args[]={NULL};
                    execv("./d1.exe",args);
                }
            }
            else if(p==2)
            {
                if(c[1]==-1)
                {
                    pipe(fd[2]);
                    pipe(fd[3]);
                    c[1]=fork();
                }
                if(c[1]>0)
                {
                    write(fd[3][1],buffer,sizeof(buffer));
                    read(fd[2][0],buffer,sizeof(buffer));
                    send(nsfd[k],buffer,sizeof(buffer),0);
                }
                else if(c[1]==0)
                {
                    dup2(fd[3][0],0);
                    dup2(fd[2][1],1);
                    char *args[]={NULL};
                    execv("./d2.exe",args);
                }
            }
            else if(p==3)
            {
                if(c[2]==-1)
                {
                    pipe(fd[4]);
                    pipe(fd[5]);
                    c[2]=fork();
                }
                if(c[2]>0)
                {
                    write(fd[5][1],buffer,sizeof(buffer));
                    read(fd[4][0],buffer,sizeof(buffer));
                    send(nsfd[k],buffer,sizeof(buffer),0);
                }
                else if(c[2]==0)
                {
                    dup2(fd[5][0],0);
                    dup2(fd[4][1],1);
                    char *args[]={NULL};
                    execv("./d3.exe",args);
                }
            }
            else if(p==4)
            {
                if(c[3]==-1)
                {
                    pipe(fd[6]);
                    pipe(fd[7]);
                    c[3]=fork();
                }
                if(c[3]>0)
                {
                    write(fd[7][1],buffer,sizeof(buffer));
                    read(fd[6][0],buffer,sizeof(buffer));
                    send(nsfd[k],buffer,sizeof(buffer),0);
                }
                else if(c[3]==0)
                {
                    dup2(fd[7][0],0);
                    dup2(fd[6][1],1);
                    char *args[]={NULL};
                    execv("./d4.exe",args);
                }
            }
        }
    }
    for(int j=1;j<=i;j++)
    close(nsfd[j-1]);
    close(sfd);
}