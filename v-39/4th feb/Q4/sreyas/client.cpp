#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include<arpa/inet.h>
#define PORT 8080
using namespace std;
int main()
{
    int sfd = 0;
    string s;
	struct sockaddr_in address,naddress;
    socklen_t add=sizeof(address);
    socklen_t nadd=sizeof(naddress);
	char buffer[1000];
	sfd=socket(AF_INET,SOCK_STREAM,0);
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_family = AF_INET;
    address.sin_port=htons(PORT);
    connect(sfd, (struct sockaddr *)&address, sizeof(address));
    cout<<"server connected"<<endl;
	while(1)
    {
        cin>>buffer;
        send(sfd,buffer,sizeof(buffer),0);
        if(int(buffer[0])>48 && int(buffer[0])<53)
        {
            cin>>buffer;
            send(sfd,buffer,sizeof(buffer),0);
        }
        cout<<buffer<<endl;
        recv(sfd,buffer,sizeof(buffer),0);
        cout<<buffer<<endl;
    }
}