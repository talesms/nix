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
#include "LoginMessage.h"
#include "CharacterListMessage.h"
#include "Avatar.h"

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
	int connectModule(MessageDelivery* msgDelivery, int portno, const char* moduleHostname, char* authorizationKey);
	int registerModule(int portno, const char* authorizationKey);
	void setSock(int sock);
	void sendToCentral(Message* message);
	int initRequestToCentral(Message* message, const char* hostname, int portno, const char* key);
	LoginMessage* requestCentraltoLoginClient(int requestSocket);
	char requestCentralToSendCharacterList(Avatar* avatarList, int requestSocket);
	void setRequestListener(int port, const char* key);


private:
	int sockfd;
	int newsockfd;
	int portno;
	socklen_t clilen;
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	char messageOffset;
	int messageSize;
	int requestListenerPort;
	char requestKey[SIZE_AUTORIZATION_KEY];
};

#endif