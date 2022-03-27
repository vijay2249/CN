#include<stdio.h>
#include<unistd.h>
#include<string.h>
int main()
{
	int pfd1[2],pfd2[2],c,kfd,sfd;
	printf("This is main\n");
	//char ibuf[20],obuf[20];
	pipe(pfd1);
	pipe(pfd2);
	
	
	c=fork();
	if(c>0)
	{
		//parent writes
		close(pfd1[0]);
		close(pfd2[1]);
		
		//dup2(10,0);
		//dup2(11,1);
		while(1)
		{
			char ibuf[20];
			sleep(1);
			printf("\nParent is writing to p2\n");
			fgets(ibuf,sizeof(ibuf),stdin);
			write(pfd1[1],ibuf,sizeof(ibuf));
			if(ibuf[0]=='1')
				break;
			sleep(1);
		}
		while(1)
		{
			char obuf[20];
			sleep(1);
			read(pfd2[0],obuf,sizeof(obuf));
			printf("parent is reading \n%s",obuf);
			if(obuf[0]=='1')
				break;
			sleep(1);
		}
	}
	else
	{
		close(pfd2[0]);
		close(pfd1[1]);
		
		dup2(0,10);
		dup2(1,11);
	
		dup2(pfd1[0],0);
		dup2(pfd2[1],1);
	
		char* arg[]={NULL};
		execv("./p4",arg);
	}
	return 0;
}