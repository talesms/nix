#include "BaseNet.h"


bool checkKey(char* originalKey, char* key);

struct ModuleThreadParam
{
	int sock;
	BaseNet* baseNet;
	MessageDelivery* messageDelivery;
};

void *moduleThreadFunction(void* moduleThreadParam)
{
	int sock = ((ModuleThreadParam*)moduleThreadParam)->sock;
	MessageDelivery* msgDelivery = ((ModuleThreadParam*)moduleThreadParam)->messageDelivery;
	Message* msg = new Message(0,'0', '0', '0', 0);
	int sizeMsg = sizeof(Message);
	BaseNet* baseNet = ((ModuleThreadParam*)moduleThreadParam)->baseNet;
	delete ((ModuleThreadParam*)moduleThreadParam);

	while(true)
	{
		if(recv(sock, (char*) msg, sizeMsg, 0) <= 0)
		{
			std::cout << "ERROR module went offline" << std::endl;
			return (void*) -1;
		}

		switch (msg->getDestination())
		{
			case MESSAGE_DESTINATION_CHAT:
			case MESSAGE_DESTINATION_LOGIN:
			case MESSAGE_DESTINATION_CREATION:
			case MESSAGE_DESTINATION_REGION:
			case MESSAGE_DESTINATION_CENTRAL:
				msgDelivery->deliverToModule(msg);
			break;
			default:
				baseNet->send(msg);
		}
		
	}
}

BaseNet::BaseNet(int portno) :
portno(portno)
{
	if(portno < 1 || portno > 65535)
		std::cout << "ERROR invalid portno." << std::endl;

	messageOffset = sizeof(int);
	messageSize = sizeof(Message) - messageOffset;
}

void BaseNet::setRequestListener(int port, const char* key)
{
	int i;

	requestListenerPort = port;
	if(requestListenerPort < 1 || requestListenerPort > 65535)
		std::cout << "ERROR invalid requestListenerPort." << std::endl;

	for(i=0; i<SIZE_AUTORIZATION_KEY; ++i)
	{
		requestKey[i] = key[i];
	}
}

BaseNet::~BaseNet()
{
	close(newsockfd);
    close(sockfd);
}

void BaseNet::send(Message* message)
{
	int sock;
	if (message != NULL)
	{
		sock = message->getClientSocket();

		//if(write(sock, message + messageOffset, messageSize) <= 0)
		if(write(sock, "teste", 6) <= 0)
		{
			std::cout << "Client " << sock << " Disconnected." << std::endl;
			close(sock);
		}
		else
			std::cout << "SUCCESS sent test to client, sock" << sock << std::endl;
	}
}

void BaseNet::sendToCentral(Message* message)
{
	write(sockfd, message, sizeof(Message));
}

int BaseNet::initRequestToCentral(Message* message, const char* hostname, int portno, const char* key)
{
    int requestSocket, n, sum_n = 0, len =0, i;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    requestSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (requestSocket < 0)
        std::cout << "ERROR opening socket at initRequestToCentral." << std::endl;
    server = gethostbyname(hostname);
    if (server == NULL) {
        std::cout << "ERROR, no such host at initRequestToCentral." << std::endl;
        return 0;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(requestSocket,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        std::cout << "ERROR connecting at initRequestToCentral." << std::endl;

    write(requestSocket, key, SIZE_AUTORIZATION_KEY);

	write(requestSocket, message, sizeof(Message));

	return requestSocket;
}

void BaseNet::listenCentral(MessageDelivery* messageDelivery)
{
	int pkgSize = sizeof(Message);
	char msg[sizeof(Message)];
	Message* message = new Message(0,'0','0','0',0);
	int n;

    std::cout << "WAITING messages from central." << std::endl;
	while(true)
	{
		n = recv(sockfd, &msg, pkgSize, 0);

		message->setClientSocket(* reinterpret_cast<int *> (msg));
		message->setDestination(msg[4]);
		message->setOption(msg[5]);
		message->setFunction(msg[6]);
		message->setValue(* reinterpret_cast<int *> (&msg[8]));

		if(n == pkgSize)
		{
			std::cout << "SUCCESS message recived from central" << std::endl;
			//write(sockfd, "ok", 2);
			messageDelivery->deliverToModule(message);
		}
		else
		{
			std::cout << "ERROR central dont respond." << std::endl;
			close(sockfd);
			return;
		}
	}
}

void* requestThreadFunction(void* param)
{
	int sock;
	Message *msg;
	char loginBuffer[60];
	Avatar avatarBuffer[10];
	int avatarNumberChosen;

	sock = (int) param;

	msg = new Message(0, 0, 0, 0, 0);

	recv(sock, msg, sizeof(Message), 0);

	if(msg->getDestination() == MESSAGE_DESTINATION_CENTRAL)
	{
		switch(msg->getOption())
		{
			case MESSAGE_OPTIONS_CENTRAL_LOGIN:
				recv(msg->getClientSocket(), loginBuffer, sizeof(char)*60, 0);
				write(sock, loginBuffer, sizeof(char)*60);
			break;
			case MESSAGE_OPTIONS_CENTRAL_CHARACTERLIST:
				recv(sock, avatarBuffer, sizeof(Avatar)*10, 0);
				write(msg->getClientSocket(), avatarBuffer, sizeof(Avatar)*10);
				recv(msg->getClientSocket(), &avatarNumberChosen, sizeof(int), 0);
				write(sock, &avatarNumberChosen, sizeof(int));
			break;
		}
	}
}

struct RequestListenerParam
{
	int portno;
	char* requestKey;
	bool (* checkKey)(char* originalKey, char* key);
};

void* requestListenerThreadFunction(void* param)
{
	int sock;
	int newsock;
	int n;
	int portno;
	socklen_t clilen;
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	pthread_t* newThread;
	char buffer[SIZE_AUTORIZATION_KEY];

	portno = ((RequestListenerParam*)param)->portno;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
		std::cout << "ERROR opening socket at request listener thread." << std::endl;

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

    if (bind(sock, (struct sockaddr *) &serv_addr,
    	sizeof(serv_addr)) < 0)
    	std::cout << "ERROR on binding at request listener thread." << errno << std::endl;

    std::cout << "WAITING Messages at request listener thread..." << std::endl;

    while(true)
	{
		listen(sock,100);
		clilen = sizeof(cli_addr);
		newsock = accept(sock, 
			(struct sockaddr *) &cli_addr, &clilen);

		if (newsock < 0)
		{
			std::cout << "ERROR on accept at request listener thread." << std::endl;
			continue;
		}

		n = recv(newsock, buffer, SIZE_AUTORIZATION_KEY, 0);

	  	if(!n)
	    	continue;

		if(((RequestListenerParam*)param)->checkKey(((RequestListenerParam*)param)->requestKey, buffer))
		{
			newThread = new pthread_t();

	  		pthread_create(newThread, NULL, requestThreadFunction, (void*) newsock);
	  	}
	}
	delete ((RequestListenerParam*)param);
}

void BaseNet::run(MessageDelivery* messageDelivery)
{
	Message* msg;
	int sock;
	pthread_t* requestListenerThread;
	RequestListenerParam* requestListenerParam;

 	requestListenerThread = new pthread_t();

 	requestListenerParam = (RequestListenerParam*) malloc(sizeof(RequestListenerParam));
 	requestListenerParam->portno = requestListenerPort;
 	requestListenerParam->checkKey = checkKey;
 	requestListenerParam->requestKey = requestKey;

 	pthread_create(requestListenerThread, NULL, requestListenerThreadFunction, requestListenerParam);


	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
		std::cout << "ERROR opening socket." << std::endl;

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

    if (bind(sock, (struct sockaddr *) &serv_addr,
    	sizeof(serv_addr)) < 0)
    	std::cout << "ERROR on binding. " << errno << std::endl;

    std::cout << "WAITING Messages..." << std::endl;

    while(true)
    {
		listen(sock,100);
		clilen = sizeof(cli_addr);
		newsockfd = accept(sock, 
			(struct sockaddr *) &cli_addr, &clilen);

		if (newsockfd < 0)
		{
			std::cout << "ERROR on accept" << std::endl;
			continue;
		}

		std::cout << "Client " << newsockfd << " IP: "
		<< inet_ntoa(cli_addr.sin_addr) << " conectou." << std::endl;

		//msg = new Message(newsockfd, MESSAGE_DESTINATION_REGION, MESSAGE_OPTIONS_REGION_AVATAR_NPC_MANAGER, MESSAGE_FUNCTION_AVATARNPCMANAGER_NEW_CONNECTION, 0);
		msg = new Message(newsockfd, MESSAGE_DESTINATION_LOGIN, 0, 0, 0);

		messageDelivery->deliverToModule(msg);
    }

    std::cout << "SERVER SHUTDOWN" << std::endl;
}

int BaseNet::connectModule(MessageDelivery* msgDelivery, int portno, const char* moduleHostname, char* authorizationKey)
{

	struct hostent *moduleHostnameHostent;
	int sockModule;
	int n;
    int sum_n = 0;
    int len =0;
    struct sockaddr_in serv_addr;
    char buffer[SIZE_AUTORIZATION_KEY];
    pthread_t* newThread;
    ModuleThreadParam* moduleThreadParam;

    moduleHostnameHostent = gethostbyname(moduleHostname);

	sockModule = socket(AF_INET, SOCK_STREAM, 0);
    if (sockModule < 0)
    {
        std::cout << "ERROR opening socket" << std::endl;
        return -1;
    }

    if (moduleHostnameHostent == NULL)
    {
        std::cout << "ERROR, no such host" << std::endl;
        return -2;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)(moduleHostnameHostent->h_addr),
         (char *)&serv_addr.sin_addr.s_addr,
         moduleHostnameHostent->h_length);
    serv_addr.sin_port = htons(portno);

    while(true)
    {
	  if (connect(sockModule,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
	  {
	    std::cout << "ERROR connecting module" << std::endl;
	  }

	  n = read(sockModule, buffer, SIZE_AUTORIZATION_KEY);
	  //write(sockModule, "ok", 2);

	  if(!n)
	    continue;

	  if(checkKey(authorizationKey, buffer))
	  {
	  	newThread = new pthread_t();
	  	moduleThreadParam = (ModuleThreadParam*) malloc(sizeof(ModuleThreadParam));
	  	moduleThreadParam->sock = sockModule;
	  	moduleThreadParam->baseNet = this;
	  	moduleThreadParam->messageDelivery = msgDelivery;


	  	pthread_create(newThread, NULL, moduleThreadFunction, (void*) moduleThreadParam);

	  	return sockModule;
	  }
	  	
	}

   return 0;
}

int BaseNet::registerModule(int portno, const char* authorizationKey)
{
	Message* msg;
	int newsock;
	struct sockaddr_in serv_addr;
	socklen_t clilen;
	char buffer[9];
	int sock;


	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
		std::cout << "ERROR opening socket." << std::endl;

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

    if (bind(sock, (struct sockaddr *) &serv_addr,
    	sizeof(serv_addr)) < 0)
    	std::cout << "ERROR on binding." << std::endl;

    std::cout << "WAITING central server..." << std::endl;

    while(true)
    {
		listen(sock,100);
		clilen = sizeof(cli_addr);
		newsock = accept(sock, 
			(struct sockaddr *) &cli_addr, &clilen);

		if (newsock < 0)
		{
			std::cout << "ERROR on accept" << std::endl;
			continue;
		}

		std::cout << "SUCCESS central server IP: "
		<< inet_ntoa(cli_addr.sin_addr) << " conectou." << std::endl;

		write(newsock, authorizationKey, SIZE_AUTORIZATION_KEY);

		std::cout << "SUCCESS connected to central server" << std::endl;
		return newsock;
    }

    return 0;    
}

bool checkKey(char* originalKey, char* key)
{
	for(int i=0; i<SIZE_AUTORIZATION_KEY; i++)
	{
	  if(originalKey[i] != key[i])
	   	return false;
	}

	return true;
}

void BaseNet::setSock(int sock)
{
	sockfd = sock;
}

LoginMessage* BaseNet::requestCentraltoLoginClient(int requestSocket)
{
	char buff[60];

	recv(requestSocket, buff, sizeof(char)*60, 0);

	close(requestSocket);

	return new LoginMessage(buff, buff+30);
}

char BaseNet::requestCentralToSendCharacterList(Avatar* avatarList, int requestSocket)
{
	char avatarNumberChosen;

	write(requestSocket, avatarList, sizeof(Avatar)*10);
	recv(requestSocket, &avatarNumberChosen, sizeof(char), 0);

	close(requestSocket);

	return avatarNumberChosen;
}
