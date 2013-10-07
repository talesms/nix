#ifndef _NIX_MESSAGEDELIVERY
#define _NIX_MESSAGEDELIVERY

#include "BaseNet.h"
#include "Message.h"
#include "Publisher.h"

class BaseNet; //Circular Dependency

class MessageDelivery : public Publisher
{
public:
	MessageDelivery(BaseNet* baseNet);
	~MessageDelivery();

	void deliverToModule(Message* message);
	void deliverToClient(Message* message);
	void deliverToCentral(Message* message);

private:
	BaseNet* baseNet;
};

#endif