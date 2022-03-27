#include<iostream>
#include<signal.h>

using namespace std;

void handler(int num){
	cout<<"signal num: "<<num;
}

int main(){
	signal(SIGUSR1, handler);
	raise(SIGUSR1);
	return 0;
}
