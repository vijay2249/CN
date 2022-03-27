#include<iostream>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
using namespace std;

int main(){
	cout<<"p1 is executing\n";
	int c;
	c = fork();
	if(c > 0){
		cout<<"p1 : child p2 is starting\n";
		system("g++ p2.cpp -o p2");
		char **s;
		execv("./p2", s);
		cout<<"p1: child p2 is terminated\n";
	}else{
		int w = 10;
		wait(&w);
		cout<<"p1: child p3 started\n";
		char **s;
		system("g++ p3.cpp -o p3");
		execv("./p3", s);
		cout<<"p1: child p3 is terminated\n";
	}
}
