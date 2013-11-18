#ifndef _NIX_INITPLAYERS
#define _NIX_INITPLAYERS

#include "../core/Configuration.h"
#include "../core/Subscriber.h"
#include "../core/Message.h"
#include "../core/MessageConfig.h"
#include "../core/MessageDelivery.h"
#include "../core/CharacterListMessage.h"
#include "../core/LoginMessage.h"

#include <iostream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>


//#define MAX_CONNECTIONS 999
#define SIZE_AUTORIZATION_KEY 14


class InitPlayers : public Subscriber
{
public:
	InitPlayers(MessageDelivery* messageDelivery);
	~InitPlayers();

	void update(Message* message);
	int connectToCacheServer();

private:
	BaseNet* baseNet;
	int cacheSock;
	MessageDelivery* messageDelivery;
};

#endif
