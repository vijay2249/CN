#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h> //for mkfifo
#include<sys/stat.h>

char inputbuffer[100];
void captalize(){
	for(int i=0; inputbuffer[i]!='\0'; i++){
		if(i==0){
			if((inputbuffer[i]>='a' && inputbuffer[i]<='z'))
			inputbuffer[i]=inputbuffer[i]-32;
			continue; 
		}
		if(inputbuffer[i]==' '){
			i++;
			if(inputbuffer[i]>='a' && inputbuffer[i]<='z'){
				inputbuffer[i]=inputbuffer[i]-32; 
				continue; 
			}
		}
		else{
			if(inputbuffer[i]>='A' && inputbuffer[i]<='Z')
			inputbuffer[i]=inputbuffer[i]+32; 
		}
	}
}

int main(){
	memset(inputbuffer,0,sizeof(inputbuffer));
	read(0,inputbuffer,sizeof(inputbuffer));
	printf("P3 Output from P2 : %s",inputbuffer);
	captalize();
	inputbuffer[strlen(inputbuffer)-1]=' ';
	strcat(inputbuffer,"-From P3");
	int r=mkfifo("GREEN",0666);
	int wffd=open("GREEN",O_WRONLY);
	write (wffd,inputbuffer,sizeof(inputbuffer));
}
