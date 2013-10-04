#include "Subscription.h"

Subscription::Subscription(Subscriber* subscriber, char type)
: subscriber(subscriber), type(type)
{
}

Subscription::~Subscription()
{
}

char Subscription::getType()
{
	return type;
}

Subscriber* Subscription::getSubscriber()
{
	return subscriber;
}