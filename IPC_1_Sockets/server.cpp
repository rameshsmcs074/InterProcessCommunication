#include<iostream>
#include<string>
#include<sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
using namespace std;

#define SOCKET_NAME "/tmp/DemocSocket.socket"

static const int invalidSocketHandle = -1;
static const int bufferLength = 128;

int main()
{
	/*-----------Everything in UNIX is a FILE --------------------------------------
	  1. Socket() - Create the master file Descriptor
	  2. Bind() - Bind that Master File Descriptor to the 
	  	 Application (i.e, is either the Port(Network domain sockets) or the path(in case of the unix domain sockets))
	  3. listen() - Start listening on that binded address or the Port
	  4. accept() - Establish the new connection with the client and create the data_scoket for the communication with client
	  5. read()/write() or send()/recev() - for the communication
	  6. close(fd) - closing the sockets (both Master fd and data_socket)*/

	int connection_scoket, data_socket;
	struct sockaddr_un name;
	int addressLength = sizeof(struct sockaddr_un);
	int returnValue;
	char buffer[128];
	int result;

	#if 0
		struct sockaddr_un {
               sa_family_t sun_family;               /* AF_UNIX */
               char        sun_path[108];            /* Pathname */
           };
    #endif

    unlink(SOCKET_NAME);

	/*int sockfd = socket(domain(AF_INET/AF_UNIX), type, protocol(0 for IP))*/ 
    connection_scoket = socket(AF_UNIX, SOCK_STREAM, 0);

    if(0 == connection_scoket)
    {
    	cout<<"Creating the Master file Descriptor failed"<<endl;
    	exit(EXIT_FAILURE);
    }

    memset(&name,0,addressLength);
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);


    /*int bind(int sockfd, const struct sockaddr *addr, 
                          socklen_t addrlen);*/
    returnValue = bind(connection_scoket,(const struct sockaddr *) &name, addressLength);

    if(invalidSocketHandle == returnValue)
    {
    	cout<<"Binding failed"<<endl;
    	exit(-1);
    }

    /*int listen(int sockfd, int backlog);*/
    returnValue = listen(connection_scoket, 3);
    if(invalidSocketHandle == returnValue)
    {
    	cout<<"listen failed"<<endl;
    	exit(-1);
    }

    for(;;)
    {
    	/*int data_socket= accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);*/
    	data_socket = accept(connection_scoket, (struct sockaddr *) &name, (socklen_t *) &addressLength);
    	if(invalidSocketHandle == returnValue)
    	{
    		cout<<"listen failed"<<endl;
    		exit(-1);
    	}

    	cout<<"connection is succeffully"<<endl;

    	for(;;)
    	{
    		cout<<"Reading the Service Request Message from the client"<<endl;
    		returnValue = read(data_socket, buffer, bufferLength);

    		result = atoi(buffer);
    		if(result == 0)
    		{
    			cout<<"result is 0"<<endl;
    			break;
    		}	
    		else
    			cout<<"Value read"<<result<<endl;	
    	}

    	close(data_socket);
    	break;
    	
    }
    
    close(connection_scoket);

    return 0;

}