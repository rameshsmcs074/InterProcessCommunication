#include<iostream>
#include<sys/socket.h>
#include<sys/un.h>
#include<unistd.h>
#include<stdlib.h>
using namespace std;

#define SOCKET_NAME "/tmp/DemoSockets.socket"

int main()
{
	int data_socket;
	struct sockaddr_un namedSocket;
	int addrLength = sizeof(struct sockaddr);
	int returnValue;
	string message;
	char ch;


	/*
           struct sockaddr_un {
               sa_family_t sun_family;               
               char        sun_path[108];            
           };*/


	data_socket = socket(AF_UNIX, SOCK_STREAM, 0);

	if(data_socket == 0)
	{
		perror("Creation on Master socket Failed");
		exit(EXIT_FAILURE);
	}

	namedSocket.sun_family = AF_UNIX;
	strncpy(namedSocket.sun_path , SOCKET_NAME, sizeof(namedSocket.sun_path)-1);

	if(connect(data_socket,(const struct sockaddr *) &namedSocket,  (socklen_t) addrLength)<0)
	{
		perror("connection Initiation Failed");
		exit(EXIT_FAILURE);
	}

	cout<<"Do you want to request something to the Server ?"<<endl;
	cin>>ch;
	for(;;)
	{
		if((ch == 'Y') || (ch == 'y'))
		{
			cout<<"Enter the Message to be sent"<<endl;
			cin>>message;
			returnValue = write(data_socket, message.c_str(), sizeof(message.c_str()));
			if(returnValue<0)
			{
				perror("Write Failed");
				exit(EXIT_FAILURE);
			}
			cout<<"Do you want to request something more to the Server ?"<<endl;
			cin>>ch;		
		}
		else
		{
			cout<<"You opted not to send anything"<<endl;
			break;
		}
	}

	close(data_socket);

	return 0;

}
