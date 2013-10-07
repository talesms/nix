#include "AvatarNpcManager.h"


struct ThreadParam
{
	int sock;
	MessageDelivery* messageDelivery;
};

void *clientThreadFunction(void* threadParam)
{
	int sock = ((struct ThreadParam*)threadParam)->sock;
	char buffer[sizeof(Message)];
	int pkgSize;
	Message* message = new Message(sock, '1', '2', '3', 4);
	MessageDelivery* messageDelivery;
	messageDelivery = ((struct ThreadParam*)threadParam)->messageDelivery;

	pkgSize = sizeof(Message);

	while(true)
	{
		sleep(3);

		messageDelivery->deliverToCentral(message);

		//if(sizeRecv <= 0)
		//{
		//	std::cout << "Client " << sock << " Disconnected." << std::endl;
		//	close(sock);
		//	pthread_exit(0);
		//}
	}
}

AvatarNpcManager::AvatarNpcManager(MessageDelivery* messageDelivery)
: messageDelivery(messageDelivery)
{
}

AvatarNpcManager::~AvatarNpcManager()
{
}

void AvatarNpcManager::update(Message* message)
{
	switch(message->getFunction())
	{
		case NEW_CONNECTION:
		newConnection(message);
		break;
	}
}

void AvatarNpcManager::newConnection(Message* message)
{
	pthread_t* newThread;
	int sock;
	struct ThreadParam* threadParam;
	threadParam = (struct ThreadParam*) malloc(sizeof(struct ThreadParam));

	newThread = new pthread_t();
	sock = message->getClientSocket();

	connections[sock] = newThread;

	threadParam->sock = sock;
	threadParam->messageDelivery = messageDelivery;

	std::cout << "SUCCESS a player has connected, sock: "<<  sock << std::endl;

	pthread_create(newThread, NULL, clientThreadFunction, (void*) threadParam);
}