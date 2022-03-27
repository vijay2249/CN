#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main(int argc, char* argv[])
{
	printf("This is p2.c\n");
    write(11,"This is p2.c\n",13);
	int rfd,wfd;
	//rfd=dup(0);
	//wfd=dup(1);

	
	while(1)
	{
		//dup2(11,1);
		sleep(1);
		char obuf[20];
		write(11,"p2 is reading\n",14);
		read(0,obuf,sizeof(obuf));
		write(11,obuf,sizeof(obuf));
		if(obuf[0]=='1')
			break;
		sleep(1);
	}
	while(1)
	{
		//dup2(10,0);
		sleep(1);
		char inbuf[20];
		write(11,"p2 is writing\n",14);
		read(10,inbuf,sizeof(inbuf));
		write(1,inbuf,sizeof(inbuf));
		if(inbuf[0]=='1')
			break;
		sleep(1);
	}
	return 0;
}