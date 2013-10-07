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