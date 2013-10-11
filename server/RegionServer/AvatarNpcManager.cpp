#include "AvatarNpcManager.h"


struct ThreadParam
{
	int sock;
	MessageDelivery* messageDelivery;
	AvatarInfo* info;
};

void *clientThreadFunction(void* threadParam)
{
	int sock = ((struct ThreadParam*)threadParam)->sock;
	char buffer[sizeof(Message)];
	int pkgSize;
	Message* message = new Message(sock, '0', '0', '0', 1);
	MessageDelivery* messageDelivery;
	AvatarInfo* localAvatarInfo = ((struct ThreadParam*)threadParam)->info;


	messageDelivery = ((struct ThreadParam*)threadParam)->messageDelivery;

	delete ((struct ThreadParam*) threadParam);

	pkgSize = sizeof(Message);

	localAvatarInfo->maxHp = 100;
	localAvatarInfo->hp = 100;
	localAvatarInfo->maxMana = 100;
	localAvatarInfo->mana = 100;

	localAvatarInfo->posX = 0;
	localAvatarInfo->posY = 0;
	localAvatarInfo->posZ = 0;

	localAvatarInfo->rotX = 0;
	localAvatarInfo->rotY = 0;
	localAvatarInfo->rotZ = 0;

	localAvatarInfo->vel = 10;

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

		//TODO: free localAvatarInfo, threadParams, message
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

	avatarInfo[sock] = (struct AvatarInfo*) malloc(sizeof(struct AvatarInfo));

	threadParam->sock = sock;
	threadParam->messageDelivery = messageDelivery;
	threadParam->info = avatarInfo[sock];

	std::cout << "SUCCESS a player has connected, sock: "<<  sock << std::endl;

	pthread_create(newThread, NULL, clientThreadFunction, (void*) threadParam);
}