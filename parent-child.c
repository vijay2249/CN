#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>

int main(){
	int pfd[2], returnStatus, c;
	char writeMsg[2][20] = {"Hi", "There"};
	char readMsg[20];
	returnStatus = pipe(pfd);
	c = fork();
	// if fork return value is ">0",the code inside this ">0" if condition belong to parent process
	if(c > 0){
	// This is parent process
		printf("This is parent process\n");
		printf("parent process => write => %s\n", writeMsg[0]);
		write(pfd[1], writeMsg[0], sizeof(writeMsg[0]));
		printf("parent process => write => %s\n", writeMsg[1]);
		write(pfd[1], writeMsg[1], sizeof(writeMsg[1]));
	}else{
		printf("This is child process\n");
		read(pfd[0], readMsg, sizeof(readMsg));
		printf("child process => read => %s\n", readMsg);
		read(pfd[0], readMsg, sizeof(readMsg));
		printf("child process => read => %s\n", readMsg);
		read(pfd[0], readMsg, sizeof(readMsg));
	}
	/*
	if(returnStatus != -1){
		printf("Writing to pipe - msg 1 => %s\n", writeMsg[0]);
		write(pfd[1], writeMsg[0], sizeof(writeMsg[0]));
		read(pfd[0], readMsg, sizeof(readMsg));
		printf("reading from pipe - msg1 => %s\n", readMsg);
		printf("writing to pipe - msg2 => %s\n", writeMsg[1]);
		write(pfd[1], writeMsg[1], sizeof(writeMsg[1]));
		read(pfd[0], readMsg, sizeof(readMsg));
		printf("reading from pipe msg-2 => %s\n", readMsg);
	}
	*/
	return 0;
}
