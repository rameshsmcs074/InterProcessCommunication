#include<iostream>
#include<sys/select.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
using namespace std;

const char *msgQName = "/msgQ1";
const int nmuOfMessages = 10;
const int maxBufferSize = nmuOfMessages*256;

int main()
{
	char buffer[maxBufferSize];
	mqd_t messageQ_Fd;
	fd_set readfds;
	struct mq_attr attributes;
	attributes.mq_maxmsg = nmuOfMessages;
	attributes.mq_msgsize = 256;

	if((messageQ_Fd = mq_open(msgQName, O_RDONLY , 0660,&attributes))<0)
	{
		perror("Opening the Message failed");
		exit(EXIT_FAILURE);
	}

	cout<<"Message queue has opened successfully"<<endl;

	while(1)
	{
		FD_ZERO(&readfds);
		FD_SET(messageQ_Fd, &readfds);

		cout<<"Waiting for the Message to be Received"<<endl;
		select(messageQ_Fd+1, &readfds, NULL, NULL, NULL);

		if(FD_ISSET(messageQ_Fd, &readfds))
		{
			cout<<"Received the Message"<<endl;
			if(mq_receive(messageQ_Fd, buffer, sizeof(buffer)+1, 0)<0)
			{
				perror("Reading failed");
				exit(EXIT_FAILURE);
			}
			else
			{
				cout<<"Message that's Received is "<<endl;
				printf("%s", buffer);
			}
		}
		else
		{
			break;
		}
	}

	close(messageQ_Fd);
}