#include<iostream>
#include<unistd.h>
using namespace std;

int main(){
  cout<<"This is one\n";
  int c = fork();
  if(c>0){//parent
    cout<<"This is two\t"<<c <<"\n";
    exit(0);
  }
  else{//child
    char **s;
    execv("./p2", s);
    cout<<"This is three\t"<<c<<"\n";
  }
  return 0;
}