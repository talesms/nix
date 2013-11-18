#include "AvatarNpcManager.h"

#ifndef NIX_THREADPARAM1
#define NIX_THREADPARAM1
struct ThreadParam
{
	int sock;
	MessageDelivery* messageDelivery;
	int cacheSock;
	int avatarId;
};
#endif

void *clientThreadFunction(void* threadParam)
{
	int sock = ((struct ThreadParam*)threadParam)->sock;
	int avatarId = ((struct ThreadParam*)threadParam)->avatarId;
	char buffer[sizeof(Message)];
	int pkgSize;
	Message* message = new Message(sock, MESSAGE_DESTINATION_CACHE, MESSAGE_OPTIONS_CACHE_AVATAR_SEARCH, '0', avatarId);
	MessageDelivery* messageDelivery;
	int cacheSock = ((struct ThreadParam*)threadParam)->cacheSock;
	Avatar* avatar =  (Avatar*) malloc(sizeof(Avatar));;

	std::cout << "Iniciando thread clientThreadFunction para o sock "<< sock << ", avatarid: " << avatarId << "." << std::endl;

	messageDelivery = ((struct ThreadParam*)threadParam)->messageDelivery;

	delete ((struct ThreadParam*) threadParam);

	pkgSize = sizeof(Message);

	write(cacheSock, message, sizeof(Message));
	read(cacheSock, avatar, sizeof(Avatar));

	std::cout << "SUCCESS o "<< avatar->name << " foi carregado com sucesso." << std::endl;

	//*/loginMessage = messageDelivery->requestCentraltoLoginClient(sock);

	//s*/messageDelivery->requestCentralToSendCharacterList(sock, avatarList);

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

int AvatarNpcManager::connectToCacheServer()
{
	int portno;
	int cacheSock;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    portno = atoi(Configuration::getConfig("cache_manager_port").c_str());
    cacheSock = socket(AF_INET, SOCK_STREAM, 0);
    if (cacheSock < 0)
        std::cout << "ERROR opening socket to connect with cache server" << std::endl;
    server = gethostbyname(Configuration::getConfig("cache_manager_ip").c_str());
    if (server == NULL)
    {
        std::cout << "ERROR, no such host to connect with cache server" << std::endl;
        return 0;
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

    write(cacheSock, Configuration::getConfig("cache_manager_key").c_str(), SIZE_AUTORIZATION_KEY);

    return cacheSock;
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
	threadParam->cacheSock = connectToCacheServer();
	threadParam->avatarId = message->getValue();
	//threadParam->info = avatarInfo[sock];

	std::cout << "SUCCESS a player has connected, sock: "<<  sock << std::endl;

	pthread_create(newThread, NULL, clientThreadFunction, (void*) threadParam);
}
