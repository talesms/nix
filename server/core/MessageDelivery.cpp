#include "MessageDelivery.h"

MessageDelivery::MessageDelivery()
//: baseNet(baseNet)
{
}

MessageDelivery::~MessageDelivery()
{
}

void MessageDelivery::deliverToModule(Message* message)
{
	notifySubscribers(message);
}

/*void MessageDelivery::deliverToClient(Message* message)
{
	baseNet->send(message);
}*/