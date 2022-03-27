#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<sys/types.h>

void close_function(int rfd,int wfd)
{
   close(rfd);
   close(wfd);
}

int main()
{
	int fd1[2],fd2[2];
	pipe(fd1);
  pipe(fd2);
    
    int child=fork();
    if(child>0)
    {
        close_function(fd1[0],fd2[1]);
        printf("**This is parent process**\n");
        printf("**Now parent to child communication is checked**\n");
        printf("**Enter the message you want to send to child**\n");

        char buff[100];
        int n;
        n=read(0,buff,100);
        write(fd1[1],buff,n);

        wait(&child);
        printf("Message received through pipe2 from child is as follows:\n");
        
        n=read(fd2[0],buff,100);

        write(1,buff,n);
    }
    else
    {
      close_function(fd2[0],fd1[1]);
      printf("**This is child process**\n");
       char buff[100];
       int n=read(fd1[0],buff,100);
      printf("Message received through pipe 1 from parent is as follows:\n");
      write(1,buff,n);
      
      printf("**Now child to parent communication is checked**\n");
      printf("**Enter the message you want to send to parent**\n");

      n=read(0,buff,100);
      
      write(fd2[1],buff,n);
    }
}