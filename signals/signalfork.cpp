#include<iostream>
#include<sys/types.h>
#include<unistd.h>
#include<signal.h>

using namespace std;

void handler1(int num){
	//cout<<"signal num: "<<num<<"\n";
	cout<<"from parent process, signal num: "<<num<<"\n";
}

void handler2(int num){
	cout<<"from child process, signal num: "<<num<<"\n";
}

int main(){
	signal(SIGUSR1, handler1);
	signal(SIGUSR2, handler2);
	pid_t p = fork();
	//parent process
	if(p > 0){
		cout<<"This is parent process of pid: "<<p<<"\n";
		//signal(SIGUSR1, handler1);
		kill(getpid(), SIGUSR2);
		//wait(100);
	}
	//child process
	else{
		cout<<"This is child process of pid: "<<p<<"\n";
		//signal(SIGUSR2, handler2);
		kill(getpid(), SIGUSR1);
	}
	return 0;
}
