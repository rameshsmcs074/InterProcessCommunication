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
	string str;
	char ch;
	mqd_t messageQ_Fd;

	//open the message queue for communication in WRONLY Mode
	if((messageQ_Fd = mq_open(msgQName, O_WRONLY | O_CREAT, 0660, 0))<0)
	{
		perror("Opening the Message Queue failed");
		exit(EXIT_FAILURE);
	}

	cout<<"Message Queue is opened for communication"<<endl;
	cout<<"Do you want to start Sending Messages? (Press 'y'/'Y' if So)"<<endl;
	cin>>ch;
	while(ch == 'y' || ch == 'Y')
	{
		cout<<"Enter the Message to be sent"<<endl;
		cin>>str;

		if((mq_send(messageQ_Fd, str.c_str(), strlen(str.c_str())+1, 0))<0)
		{
			perror("Sending message failed");
			exit(EXIT_FAILURE);
		}

		cout<<"Do you want to continue Sending Messages"<<endl;
		cin>>ch;
	}

	mq_close(messageQ_Fd);
	mq_unlink(msgQName);
	return 0;

}