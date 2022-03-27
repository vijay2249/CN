#include<stdio.h>
int main(){
	int a;
	a = printf("hello world\n");
	printf("%d\n", a);
	switch(a){
		case 1: printf("1");
		case 11:printf("2");
		case 12:printf("3");
		default: printf("4");
	}
}
