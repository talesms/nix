#include "Message.h"

Message::Message(int clientSocket, char destination, char option, char function, int value)
: clientSocket(clientSocket), destination(destination), option(option), function(function), value(value)
{
}

Message::Message(int clientSocket, char* data)
: clientSocket(clientSocket)
{
	int charSize = sizeof(char);
	destination = (char) *(data);
	option = (char) *(data + charSize);
	function = (char) *(data + 2*charSize);
	value = (int) *(data + 3*charSize);
}

int Message::getClientSocket()
{
	return clientSocket;
}

char Message::getDestination()
{
	return destination;
}


char Message::getOption()
{
	return option;
}

char Message::getFunction()
{
	return function;
}

int Message::getValue()
{
	return value;
}