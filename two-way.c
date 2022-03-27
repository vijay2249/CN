#include<stdio.h>
#include<unistd.h>

int main(){
	int pfd1[2], pfd2[2];
	if(pipe(pfd1) == -1 || pipe(pfd2) == -1){
		printf("unable to create pipes. Terminating the program\n");
		return 1;
	}
	int c = fork();
	char buffer1[30],buffer2[30], buffer3[30], buffer4[30];
	if(c > 0){
		// parent process
		close(pfd1[0]); //close read of pipe 1
		close(pfd2[1]); //close write of pipe 2
		while(1){
			printf("Parent process input: ");
			fgets(buffer1, sizeof(buffer1), stdin);
			write(pfd1[1], buffer1, sizeof(buffer1));
			read(pfd2[0], buffer2, sizeof(buffer2));
			printf("\nChild output inside parent process : %s\n", buffer2);
		}
	}
	else{
		// child process
		close(pfd1[1]); //close write of pipe 1
		close(pfd2[0]); //close read of pipe 2
		while(1){
			read(pfd1[0], buffer3, sizeof(buffer3));
			printf("\nParent output inside child process : %s\n", buffer3);
			printf("Child process input: ");
			fgets(buffer4, sizeof(buffer4), stdin);
			write(pfd2[1], buffer4, sizeof(buffer4));
		}
	}
	
	return 0;
}
