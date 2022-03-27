#include<iostream>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>

using namespace std;

void handler1(int num){
	cout<<"from parent, signal num: "<<num<<"\n";
}
void handler2(int num){
	cout<<"from child, signal num: "<<num<<"\n";
}

int main(){
	mkfifo("serve", 0666);
	signal(SIGUSR1, handle1);
	signal(SIGUSR2, handle2);
	int fd = open("serve", O_RDONLY);
	char c, *msg;
	int j=0;
	while(read(fd, &c, 1)){
		msg[j++] = c;
	}
	msg[j] = '\0';
	while(getpid() == atoi(msg)){
		j=0;
		while(read(fd, &c, 1)){
			msg[j++] = c;
		}
	}


	return 0;

}
