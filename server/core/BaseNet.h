#ifndef NIX_BASENET
#define NIX_BASENET

#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>

#include "Message.h"
#include "MessageConfig.h"
#include "MessageDelivery.h"

#define SIZE_AUTORIZATION_KEY 14

class MessageDelivery; //Circular Dependency

class BaseNet
{
public:
	BaseNet(int portno);
	~BaseNet();
	void run(MessageDelivery* messageDelivery);
	void listenCentral(MessageDelivery* messageDelivery);
	void send(Message* message);
	int connectModule(int portno, const char* moduleHostname, char* authotizationKey);
	int registerModule(int portno, const char* authotizationKey);
	void setSock(int sock);
	void sendToCentral(Message* message);

private:
	int sockfd;
	int newsockfd;
	int portno;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	char messageOffset;
	int messageSize;

	bool checkKey(char* originalKey, char* key);
};

#endif