#include "LoginMessage.h"

LoginMessage::LoginMessage(int clientSocket, char* user, char* pass)
: clientSocket(clientSocket)
{
	int i;
	for(i=0; user[i] != '\0'; ++i)
		username[i] = user[i];

	username[i] = '\0';


	for(i=0; pass[i] != '\0'; ++i)
		password[i] = pass[i];

	password[i] = '\0';
}

LoginMessage::~LoginMessage()
{

}

int LoginMessage::getClientSocket()
{
	return clientSocket;
}

char* LoginMessage::getUsername()
{
	return username;
}

char* LoginMessage::getPassword()
{
	return password;
}