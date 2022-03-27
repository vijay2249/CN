
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/wait.h>
int main()
{
	// printf("\n1This is parent process %d.",(int)getpid());
    printf("%s\n","1This is parent process");
	int c=fork();
	if(c>0)
	{
		printf("\n2This is parent process");
		wait(NULL);
		printf("\n4Child process terminated");
	}
	else
	{
		printf("\n3This is child process");
		//execv("p2.exe");
		printf("\n4Child process %d terminated\n",(int)getpid());
	}
	return 0;
}