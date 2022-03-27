#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int main(){
	int n1, n2,s;
	int in_fd = open("./input.txt", O_RDONLY);
	int out_fd = open("./output.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	dup2(in_fd, 0);
	dup2(out_fd, 1);
	scanf("%d %d", &n1, &n2);
	s = n1 + n2;
	printf("%d + %d = %d\n", n1, n2, s);
	return EXIT_SUCCESS;
}
