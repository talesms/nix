#include "MessageDelivery.h"

MessageDelivery::MessageDelivery(BaseNet* baseNet)
: baseNet(baseNet)
{
}

MessageDelivery::~MessageDelivery()
{
}

void MessageDelivery::deliverToModule(Message* message)
{
	notifySubscribers(message);
}

void MessageDelivery::deliverToClient(Message* message)
{
	baseNet->send(message);
}

void MessageDelivery::deliverToCentral(Message* message)
{
	baseNet->sendToCentral(message);
}

void MessageDelivery::setCentralRequestListener()
{
	requestHostname = (char*) malloc(sizeof(char)*16);
	requestKey = (char*) malloc(sizeof(char)*SIZE_AUTORIZATION_KEY);

	for(int i=0; i<16; ++i)
	{
		if((requestHostname[i] = Configuration::getConfig("frontend_server_ip").c_str()[i]) == '\0')
			break;
	}

	for(int i=0; i<SIZE_AUTORIZATION_KEY; ++i)
	{
		if((requestKey[i] = Configuration::getConfig("frontend_server_request_key").c_str()[i]) == '\0')
			break;
	}

	requestPortno = atoi(Configuration::getConfig("frontend_server_request_port").c_str());
}

LoginMessage* MessageDelivery::requestCentraltoLoginClient(int clientSock)
{
	int requestSocket;

	Message* message = new Message(clientSock, MESSAGE_DESTINATION_CENTRAL, MESSAGE_OPTIONS_CENTRAL_LOGIN, '0', 1);

	requestSocket = baseNet->initRequestToCentral(message, requestHostname, requestPortno, requestKey);

	return baseNet->requestCentraltoLoginClient(requestSocket);
}

char MessageDelivery::requestCentralToSendCharacterList(int clientSock, CharacterListMessage* characterListMessage)
{
	int requestSocket;

	Message* message = new Message(clientSock, MESSAGE_DESTINATION_CENTRAL, MESSAGE_OPTIONS_CENTRAL_CHARACTERLIST, '0', 1);
	requestSocket = baseNet->initRequestToCentral(message, requestHostname, requestPortno, requestKey);

	return baseNet->requestCentralToSendCharacterList(characterListMessage->getCharacterList(), requestSocket);
}
