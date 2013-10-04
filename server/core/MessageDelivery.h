#ifndef _NIX_MESSAGEDELIVERY
#define _NIX_MESSAGEDELIVERY

//#include "BaseNet.h"
#include "Message.h"
#include "Publisher.h"

class MessageDelivery : public Publisher
{
public:
	MessageDelivery();
	~MessageDelivery();

	void deliverToModule(Message* message);
	//void deliverToClient(Message* message);

private:
	//BaseNet* baseNet;
};

#endif