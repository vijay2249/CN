#include<iostream>
#include<sys/types.h>
#include<unistd.h>
using namespace std;

int main(){
	cout<<"p1 is executing\n";
	char **s;
	execv("./p2", s);
}
