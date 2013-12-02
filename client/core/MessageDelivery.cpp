#include "MessageDelivery.h"

MessageDelivery::MessageDelivery(Network* network)
: network(network)
{
}

MessageDelivery::~MessageDelivery()
{
}

void MessageDelivery::deliverToModule(Message* message)
{
	notifySubscribers(message);
}

void MessageDelivery::deliverToServer(Message* message)
{
	baseNet->send(message);
}
