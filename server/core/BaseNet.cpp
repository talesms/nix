#include "BaseNet.h"

#define PKG_SIZE 1024


void BaseNet::send(Message* message)
{
	int sock;
	if (message != NULL)
	{
		sock = message->getClientSocket();

		if(write(sock, message + messageOffset, messageSize) <= 0)
		{
			std::cout << "Client " << sock << " Disconnected." << std::endl;
			close(sock);
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

void BaseNet::listenCentral(MessageDelivery* messageDelivery)
{
	int pkgSize = sizeof(Message);
	char msg[sizeof(Message)];
	int n;

    std::cout << "WAITING messages from central." << std::endl;
	while(true)
	{
		n = recv(sockfd, msg, pkgSize, 0);
		if(n == pkgSize)
		{
			write(sockfd, "ok", 2);
			messageDelivery->deliverToModule((Message*)msg);
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

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		std::cout << "ERROR opening socket." << std::endl;

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr,
    	sizeof(serv_addr)) < 0)
    	std::cout << "ERROR on binding. " << errno << std::endl;

    std::cout << "WAITING Messages..." << std::endl;

    while(true)
    {
		listen(sockfd,100);
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, 
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
	int n;
    int sum_n = 0;
    int len =0;
    struct sockaddr_in serv_addr;
    char buffer[SIZE_AUTORIZATION_KEY];

    moduleHostnameHostent = gethostbyname(moduleHostname);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
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
	  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
	  {
	    std::cout << "ERROR connecting" << std::endl;
	  }

	  n = read(sockfd,buffer,SIZE_AUTORIZATION_KEY);
	  write(sockfd, "Recebido", 9);

	  if(!n)
	    continue;

	  if(checkKey(authotizationKey, buffer))
	  	return sockfd;
	}

   return 0;
}

int BaseNet::registerModule(int portno, const char* authotizationKey)
{
	Message* msg;
	int newsock;
	struct sockaddr_in serv_addr;
	socklen_t clilen;
	int n;
	char buffer[9];


	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		std::cout << "ERROR opening socket." << std::endl;

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr,
    	sizeof(serv_addr)) < 0)
    	std::cout << "ERROR on binding." << std::endl;

    std::cout << "WAITING central server..." << std::endl;

    while(true)
    {
		listen(sockfd,100);
		clilen = sizeof(cli_addr);
		newsock = accept(sockfd, 
			(struct sockaddr *) &cli_addr, &clilen);

		if (newsock < 0)
		{
			std::cout << "ERROR on accept" << std::endl;
			continue;
		}

		std::cout << "SUCCESS central server IP: "
		<< inet_ntoa(cli_addr.sin_addr) << " conectou." << std::endl;

		write(newsock, authotizationKey, SIZE_AUTORIZATION_KEY);
		n = read( newsock, buffer, 9);

		if(n>0)
		{
			std::cout << "SUCCESS connected to central server" << std::endl;
			return newsock;
		}
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

BaseNet::~BaseNet()
{
	close(newsockfd);
    close(sockfd);
}