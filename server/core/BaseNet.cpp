#include "BaseNet.h"

//#define PKG_SIZE 1024

struct ModuleThreadParam
{
	int sock;
	BaseNet* baseNet;
};

void *moduleThreadFunction(void* moduleThreadParam)
{
	int sock = ((ModuleThreadParam*)moduleThreadParam)->sock;
	Message* msg = new Message(0,'0', '0', '0', 0);
	int sizeMsg = sizeof(Message);
	BaseNet* baseNet = ((ModuleThreadParam*)moduleThreadParam)->baseNet;

	while(true)
	{
		if(recv(sock, (char*) msg, sizeMsg, 0) <= 0)
		{
			std::cout << "ERROR module went offline" << std::endl;
			return (void*) -1;
		}

		baseNet->send(msg);
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
	int n;
	n = write(sockfd, message, sizeof(Message));
	std::cout << "DEBUG sent " << n << " to central, sock " << sockfd << std::endl;
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

void BaseNet::run(MessageDelivery* messageDelivery)
{
	Message* msg;
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

		msg = new Message(newsockfd, REGION, AVATAR_NPC_MANAGER, NEW_CONNECTION, 0);

		messageDelivery->deliverToModule(msg);
    }

    std::cout << "SERVER SHUTDOWN" << std::endl;
}

int BaseNet::connectModule(int portno, const char* moduleHostname, char* authotizationKey)
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

	  if(checkKey(authotizationKey, buffer))
	  {
	  	newThread = new pthread_t();
	  	moduleThreadParam = (ModuleThreadParam*) malloc(sizeof(ModuleThreadParam));
	  	moduleThreadParam->sock = sockModule;
	  	moduleThreadParam->baseNet = this;


	  	pthread_create(newThread, NULL, moduleThreadFunction, (void*) moduleThreadParam);

	  	return sockModule;
	  }
	  	
	}

   return 0;
}

int BaseNet::registerModule(int portno, const char* authotizationKey)
{
	Message* msg;
	int newsock;
	struct sockaddr_in serv_addr;
	socklen_t clilen;
	//int n;
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

		write(newsock, authotizationKey, SIZE_AUTORIZATION_KEY);
		//n = read(newsock, buffer, 2);

		//if(n>0)
		//{
			std::cout << "SUCCESS connected to central server" << std::endl;
			return newsock;
		//}
    }

    return 0;    
}

bool BaseNet::checkKey(char* originalKey, char* key)
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