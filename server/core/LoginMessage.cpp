#include "LoginMessage.h"

LoginMessage::LoginMessage(char* user, char* pass)
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

char* LoginMessage::getUsername()
{
	return username;
}

char* LoginMessage::getPassword()
{
	return password;
}