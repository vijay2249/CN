#include<iostream>
#include<sys/types.h>
#include<unistd.h>
#include<signal.h>

using namespace std;

void handler(int num){
	cout<<"signal num: "<<num;
}

int main(){
	signal(SIGUSR1, handler);
	kill(getpid(),SIGUSR1);
	return 0;
}
