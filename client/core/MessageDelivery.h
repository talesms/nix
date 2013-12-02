#ifndef _NIX_MESSAGEDELIVERY
#define _NIX_MESSAGEDELIVERY

#include "Message.h"
#include "Publisher.h"
#include "Configuration.h"
#include "../Network.h"


class MessageDelivery : public Publisher
{
public:
	MessageDelivery(Network* network);
	~MessageDelivery();

	void deliverToModule(Message* message);
	void deliverToServer(Message* message);

private:
	Network* network;
};

#endif
