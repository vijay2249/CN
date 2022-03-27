#include<iostream>
#include<signal.h>

using namespace std;

void handler(int num){
	cout<<"signal number: "<<num<< "\n";
}

int main(){
	signal(SIGINT, handler);
	int temp = 1;
	while(temp) {cin>>temp;temp++;}
	return 0;
}
