#include<iostream>
#include<sys/socket.h>
#include<sys/un.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/select.h>
#include<set>
using namespace std;

#define SOCKET_NAME "/tmp/DemoSockets.socket"
set<int> setOfConnectedSockets;


void add_fds(int fd)
{
	setOfConnectedSockets.insert(fd);
}

void refereshReadFds(fd_set *readfds)
{
	FD_ZERO(readfds);
	for(int i : setOfConnectedSockets)
	{
		FD_SET(i, readfds);	
	}
}

int getMaxFds()
{
	int maxFD = 0;
	for(int i: setOfConnectedSockets)
	{
		maxFD = max(maxFD, i);
	}

	return maxFD;
}



int main()
{
	int master_fd, data_socket;
	struct sockaddr_un namedSocket;
	int addrLength = sizeof(struct sockaddr);
	int returnValue;
	char buffer[255];
	
	fd_set readFds;

	/*

           struct sockaddr_un {
               sa_family_t sun_family;               
               char        sun_path[108];            
           };*/


	master_fd = socket(AF_UNIX, SOCK_STREAM, 0);

	if(master_fd == 0)
	{
		perror("Creation on Master socket Failed");
		exit(EXIT_FAILURE);
	}

	namedSocket.sun_family = AF_UNIX;
	strncpy(namedSocket.sun_path , SOCKET_NAME, sizeof(namedSocket.sun_path)-1);

	if(bind(master_fd,(const struct sockaddr *) &namedSocket,  (socklen_t) addrLength)<0)
	{
		perror("Bind Failed");
		exit(EXIT_FAILURE);
	}

	//adding the master fd to readfds
	add_fds(master_fd);


	if(listen(master_fd, 4)<0)
	{
		perror("Listen Failed");
		exit(EXIT_FAILURE);
	}

	for(;;)
	{
		refereshReadFds(&readFds);

		cout<<"Waiting for Clients to Either request for CIR or SRM"<<endl;
		select(getMaxFds()+1, &readFds, NULL, NULL, NULL);

		if(FD_ISSET(master_fd, &readFds) >0)
		{
			cout<<"Connection Initaion Message is received"<<endl;

			data_socket = accept(master_fd, ( struct sockaddr *)&namedSocket, (socklen_t *) &addrLength);
			if(data_socket<0)
			{
				perror("Accept failed");
				exit(EXIT_FAILURE);
			}
			add_fds(data_socket);
			cout<<"Connection is Esatblished with Client having Fd "<<data_socket<<endl;
		}
		else 
		{
			int CurrentFd;
			for(auto j: setOfConnectedSockets)
			{
				if(FD_ISSET(j, &readFds)>0)
				{
					CurrentFd = j;
					break;
				}
			}

			cout<<"SRM is received from Client Fd"<<CurrentFd<<endl;

			returnValue = read(CurrentFd, buffer, sizeof(buffer)-1);
			string str = buffer;

			cout<<"Client Fd "<<CurrentFd<<" has sent the message "<<str<<endl;
		}
	}
	close(master_fd);
	return 0;

}
