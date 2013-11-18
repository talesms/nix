#ifndef _NIX_MESSAGEDELIVERY
#define _NIX_MESSAGEDELIVERY

#include "BaseNet.h"
#include "Message.h"
#include "Publisher.h"
#include "LoginMessage.h"
#include "CharacterListMessage.h"
#include "Configuration.h"

#define SIZE_AUTORIZATION_KEY 14

class BaseNet; //Circular Dependency

class MessageDelivery : public Publisher
{
public:
	MessageDelivery(BaseNet* baseNet);
	~MessageDelivery();

	void deliverToModule(Message* message);
	void deliverToClient(Message* message);
	void deliverToCentral(Message* message);
	void setCentralRequestListener();
	LoginMessage* requestCentraltoLoginClient(int clientSock);
	char requestCentralToSendCharacterList(int clientSock, CharacterListMessage* characterListMessage);

private:
	BaseNet* baseNet;
	char* requestHostname;
	int requestPortno;
	char* requestKey;
};

#endif
