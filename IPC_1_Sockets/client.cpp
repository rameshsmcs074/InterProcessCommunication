#include<iostream>
#include<string>
#include<sys/socket.h>
#include<sys/un.h>
#include<unistd.h>
using namespace std;

#define SOCKET_NAME "/tmp/DemocSocket.socket"

static const int invalidSocketHandle = -1;
int value = 5;

int main()
{
	/*-----------Everything in UNIX is a FILE --------------------------------------
	  1. Socket() - Create the master file Descriptor
      2. connect() - Connection Iniation Message from the client to Server
	  2. read()/write() or send()/recev() - for the communication
	  3. close(fd) - closing the sockets (both Master fd and data_socket)*/

	int connection_scoket, data_socket;
	struct sockaddr_un name;
	int addressLength = sizeof(sockaddr_un);
	int returnValue;
    char buffer[128];

	#if 0
		struct sockaddr_un {
               sa_family_t sun_family;               /* AF_UNIX */
               char        sun_path[108];            /* Pathname */
           };
    #endif

	/*int sockfd = socket(domain(AF_INET/AF_UNIX), type, protocol(0 for IP))*/ 
    data_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if(0 == connection_scoket)
    {
    	cout<<"Creating the Master file Descriptor failed"<<endl;
    	exit(EXIT_FAILURE);
    }

    memset(&name,0,addressLength);
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);


    /*int connect(int sockfd, const struct sockaddr *addr,  
                             socklen_t addrlen);*/
    returnValue = connect(data_socket,(struct sockaddr *) &name, addressLength);

    if(invalidSocketHandle == returnValue)
    {
    	cout<<"connection failed"<<endl;
    	exit(-1);
    }

    while(value)
    {
        cout<<"Enter the number to be sent"<<endl;
        cin>>buffer;
        int ret = send(data_socket, buffer, sizeof(buffer), 0);
        if(-1 == ret)
        {
            cout<<"send failed"<<endl;
            exit(EXIT_FAILURE);
        }
        {
            cout<<"Message sent"<<endl;
        }
        --value;
    }


    close(data_socket);

    return 0;

}