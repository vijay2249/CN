#include<iostream>
#include<signal.h>

using namespace std;

int main(){
	int temp = 1;
	while(temp < 10000000){
		cout<<"temp: "<<temp++<<endl;
	}
	return 0;
}
