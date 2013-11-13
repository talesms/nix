#include "AvatarNpcManager.h"


struct ThreadParam
{
	int sock;
	MessageDelivery* messageDelivery;
	int cacheSock;
	//AvatarInfo* info;
};

void *clientThreadFunction(void* threadParam)
{
	int sock = ((struct ThreadParam*)threadParam)->sock;
	char buffer[sizeof(Message)];
	int pkgSize;
	Message* message = new Message(sock, '0', '0', '0', 1);
	MessageDelivery* messageDelivery;
	//AvatarInfo* localAvatarInfo = ((struct ThreadParam*)threadParam)->info;


	messageDelivery = ((struct ThreadParam*)threadParam)->messageDelivery;

	delete ((struct ThreadParam*) threadParam);

	pkgSize = sizeof(Message);

	//if(!messageDelivery->loginStep(sock))
	//	return;

	//messageDelivery->characterListStep(sock);

	/*localAvatarInfo->maxHp = 100;
	localAvatarInfo->hp = 100;
	localAvatarInfo->maxMana = 100;
	localAvatarInfo->mana = 100;

	localAvatarInfo->posX = 0;
	localAvatarInfo->posY = 0;
	localAvatarInfo->posZ = 0;

	localAvatarInfo->rotX = 0;
	localAvatarInfo->rotY = 0;
	localAvatarInfo->rotZ = 0;

	localAvatarInfo->vel = 10;*/

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
	connectToCacheServer();
}

AvatarNpcManager::~AvatarNpcManager()
{
}

void AvatarNpcManager::connectToCacheServer()
{
	int portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    portno = atoi(Configuration::getConfig("cache_manager_port").c_str());
    cacheSock = socket(AF_INET, SOCK_STREAM, 0);
    if (cacheSock < 0)
        std::cout << "ERROR opening socket to connect with cache server" << std::endl;
    server = gethostbyname(Configuration::getConfig("cache_manager_ip").c_str());
    if (server == NULL) {
        std::cout << "ERROR, no such host to connect with cache server" << std::endl;
        return;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(cacheSock,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
         std::cout << "ERROR connecting with cache server" << std::endl;
    else
    	std::cout << "SUCCESS connected to cache server" << std::endl;
}

void AvatarNpcManager::update(Message* message)
{
	switch(message->getFunction())
	{
		case MESSAGE_FUNCTION_AVATARNPCMANAGER_NEW_CONNECTION:
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

	//avatarInfo[sock] = (struct AvatarInfo*) malloc(sizeof(struct AvatarInfo));

	threadParam->sock = sock;
	threadParam->messageDelivery = messageDelivery;
	//threadParam->info = avatarInfo[sock];

	std::cout << "SUCCESS a player has connected, sock: "<<  sock << std::endl;

	pthread_create(newThread, NULL, clientThreadFunction, (void*) threadParam);
}