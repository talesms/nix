#include "Message.h"

Message::Message(int clientSocket, char destination, char option, char function, int value)
: clientSocket(clientSocket), destination(destination), option(option), function(function), value(value)
{
}

int Message::getClientSocket()
{
	return clientSocket;
}

void Message::setClientSocket(int clientSocket)
{
	this->clientSocket = clientSocket;
}

char Message::getDestination()
{
	return destination;
}

void Message::setDestination(char destination)
{
	this->destination = destination;
}

char Message::getOption()
{
	return option;
}

void Message::setOption(char option)
{
	this->option = option;
}

char Message::getFunction()
{
	return function;
}

void Message::setFunction(char function)
{
	this->function = function;
}

int Message::getValue()
{
	return value;
}

void Message::setValue(int value)
{
	this->value = value;
}