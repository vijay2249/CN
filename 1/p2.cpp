#include<iostream>
#include<sys/types.h>
#include<unistd.h>
using namespace std;

int main(){
	cout<<"p2 is executing\n";
	char **s;
	execv("./p3", s);
}
