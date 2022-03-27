#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/wait.h>
#include<sys/types.h>

int main(int argc, char *argv[])
{
	int c,stat;
	pid_t p2;
	printf("%s\n","1 This is parent process.");
	c=fork();
	if(c>0)
	{
		printf("%s\n","2 This is parent process");
		int c2=fork();
		if(c2>0)
		{
		    pid_t p=wait(&stat);
		    printf("Child process terminated is %d\n",(int)p);
		    pid_t pid=waitpid(p,&stat,0);
		    printf("this part of code is executed only after p3 terminates\n");
		 }
		 else
		 {
		 	char* arg[]={NULL};
			execv("./p3",arg);
		}
		p2=wait(&stat);
		printf("Child process terminated is %d\n",(int)p2);
	}
	else
	{
		printf("%s\n","3 This is child process");
		char* arg[]={NULL};
		execv("./p2",arg);
		printf("%s\n","4 Child process terminated");
	}
	return 0;
}