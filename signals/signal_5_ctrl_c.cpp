#include<iostream>
#include<signal.h>

using namespace std;

int count = 0;

void handler(int signal_no){
	count++;
	if(count == 5) {
		cout<<"pressed ctrl c 5 times\n";
		signal(SIGINT, SIG_DFL);
	}
}

int main(){
	signal(SIGINT, handler);
	int temp = 1;
	while(temp){
		cin>>temp;
		temp++;
	}
	return 0;
}
