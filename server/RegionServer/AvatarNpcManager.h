#ifndef _NIX_AVATARNPCMANAGER
#define _NIX_AVATARNPCMANAGER

#include "../core/Subscriber.h"
#include "../core/Message.h"
#include "../core/MessageConfig.h"

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

#define MAX_CONNECTIONS 9999

class AvatarNpcManager : public Subscriber
{
public:
	AvatarNpcManager();
	~AvatarNpcManager();

	void update(Message* message);

private:
	pthread_t* connections[MAX_CONNECTIONS]; //TODO: criar funcao hash para mapear o socket dentro do size

	void newConnection(Message* message);
	void debugMessage(Message* message); //TODO: testando, retirar
};

#endif