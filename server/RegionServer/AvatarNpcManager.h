#ifndef _NIX_AVATARNPCMANAGER
#define _NIX_AVATARNPCMANAGER

#include "../core/Subscriber.h"
#include "../core/Message.h"
#include "../core/MessageConfig.h"
#include "../core/MessageDelivery.h"

//TODO: Testando, retirar esse include
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

#define MAX_CONNECTIONS 9999


class AvatarNpcManager : public Subscriber
{
public:
	AvatarNpcManager(MessageDelivery* messageDelivery);
	~AvatarNpcManager();

	virtual void update(Message* message);

private:
	pthread_t* connections[MAX_CONNECTIONS]; //TODO: criar funcao hash para mapear o socket dentro do size
	//AvatarInfo* avatarInfo[MAX_CONNECTIONS];
	MessageDelivery* messageDelivery;



	void newConnection(Message* message);
};

#endif