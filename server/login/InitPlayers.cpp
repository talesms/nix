#include "InitPlayers.h"

#ifndef NIX_THREADPARAM1
#define NIX_THREADPARAM1
struct ThreadParam
{
	int sock;
	MessageDelivery* messageDelivery;
	InitPlayers* initPlayers;
};
#endif

void *newClientThreadFunction(void* threadParam);


InitPlayers::InitPlayers(MessageDelivery* messageDelivery)
: messageDelivery(messageDelivery)
{
}

InitPlayers::~InitPlayers()
{

}

int InitPlayers::connectToCacheServer()
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

void InitPlayers::update(Message* message)
{
	pthread_t* newThread;
	int sock;
	struct ThreadParam* threadParam;
	threadParam = (struct ThreadParam*) malloc(sizeof(struct ThreadParam));

	newThread = new pthread_t();
	sock = message->getClientSocket();

	threadParam->sock = sock;
	threadParam->messageDelivery = messageDelivery;
	threadParam->initPlayers = this;

	std::cout << "SUCCESS a player has connected, sock: " <<  sock << std::endl;

	pthread_create(newThread, NULL, newClientThreadFunction, (void*) threadParam);
}

void *newClientThreadFunction(void* threadParam)
{
	int sock = ((struct ThreadParam*)threadParam)->sock;
	char buffer[sizeof(Message)];
	Message* message = new Message(sock, MESSAGE_DESTINATION_CACHE, MESSAGE_OPTIONS_CACHE_LOGIN, '0', 1);
	MessageDelivery* messageDelivery;
	int cacheSock;
	int n = 0;
	char avatarNumberChosen;
	LoginMessage* loginMessage;
	CharacterListMessage* characterListMessage = (CharacterListMessage*) malloc(sizeof(CharacterListMessage));
	InitPlayers* initPlayers = ((struct ThreadParam*)threadParam)->initPlayers;
	Message* msgNewConnectionAvatarNpcManager;

	messageDelivery = ((struct ThreadParam*)threadParam)->messageDelivery;

	delete ((struct ThreadParam*) threadParam);

	cacheSock = initPlayers->connectToCacheServer();

	if (cacheSock <= 0)
		return (void*) -1;;

	loginMessage = messageDelivery->requestCentraltoLoginClient(sock);

	write(cacheSock, message, sizeof(Message));
	write(cacheSock, loginMessage, sizeof(LoginMessage));
	n = recv(cacheSock, characterListMessage, sizeof(CharacterListMessage), 0);

	if (n <= 0)
		return (void*) -1;

	avatarNumberChosen = messageDelivery->requestCentralToSendCharacterList(sock, characterListMessage);

	msgNewConnectionAvatarNpcManager = new Message(sock, MESSAGE_DESTINATION_REGION, MESSAGE_OPTIONS_REGION_AVATAR_NPC_MANAGER, 
		MESSAGE_FUNCTION_AVATARNPCMANAGER_NEW_CONNECTION, characterListMessage->getCharacter(avatarNumberChosen).idavatar);

	messageDelivery->deliverToCentral(msgNewConnectionAvatarNpcManager);
}
