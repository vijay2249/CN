#include<iostream>
#include<sys/types.h>
#include<unistd.h>
using namespace std;

int main(){
	cout<<"p3 is executing\n";
	char **s;
	execv("./p4", s);
}
