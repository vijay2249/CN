#include<iostream>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

using namespace std;

int main(){
	mkfifo("server", 0666);
	mkfifo("Client1", 0666);
	mkfifo("Client2", 0666);
	mkfifo("Client3", 0666);
	while(1){
		int server_fd = open("server", O_RDONLY);
		cout<<"Server FIFO is opened\n";
		char c, msg[2][1024];
		int j=0, d = 0;
		while(read(server_fd, &c, 1)){
			if(c == '$'){
				msg[d][j++] = '\0';
				j=0;
				d++;
			}
			else{ msg[d][j++] = c; }
		}
		cout<<"Msg read from grp by server: \n";
		for(int i=0;i<2;i++){
			int k=0;
			//while(msg[i][k++] != '\0') printf("%c", msg[1][k]);
			while(msg[i][k++] != '\0') cout<<msg[1][k];
			cout<<"\n";
		}
		int fd1 = open("Client1", O_WRONLY);
		int fd2 = open("Client2", O_WRONLY);
		int fd3 = open("Client3", O_WRONLY);
		write(fd3, msg[0], 1024);
		write(fd2, msg[0], 1024);
		write(fd1, msg[0], 1024);

		read(fd3, msg[1], 1024);
		read(fd2, msg[1], 1024);
		read(fd1, msg[1], 1024);
		
		close(fd1);
		close(fd2);
		close(fd3);
		close(server_fd);
	}
	return 0;
}
