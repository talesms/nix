#include "AvatarNpcManager.h"

void *clientThreadFunction(void* clienSock)
{
	int sock = (int) clienSock;
	int sizeWrite;
	int sizeRecv;
	char buffer[sizeof(Message)];
	int pkgSize;

	pkgSize = sizeof(Message);

	while(true)
	{
		sleep(3);

		sizeWrite = write(sock, "teste" , 6);
		std::cout << "Enviado " << sizeWrite << " para "<< sock << "." << std::endl;

		sizeRecv = recv(sock, buffer, pkgSize, 0);

		if(sizeRecv <= 0)
		{
			std::cout << "Client " << sock << " Disconnected." << std::endl;
			close(sock);
			pthread_exit(0);
		}
	}
}

AvatarNpcManager::AvatarNpcManager()
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

void AvatarNpcManager::debugMessage(Message* message)
{
	std::cout << "message->clientSocket: " << message->getClientSocket() << std::endl;
	std::cout << "message->destination: " << message->getDestination() << std::endl;
	std::cout << "message->function: " << message->getFunction() << std::endl;
	std::cout << "message->value: " << message->getValue() << std::endl;
}

void AvatarNpcManager::newConnection(Message* message)
{
	pthread_t* newThread;
	int sock;

	newThread = new pthread_t();
	sock = message->getClientSocket();

	connections[message->getClientSocket()] = newThread;

	std::cout << "SUCCESS a player has connected, sock: "<<  sock << std::endl;

	pthread_create(newThread, NULL, clientThreadFunction, (void*) sock);
}