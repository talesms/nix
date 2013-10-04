#ifndef _NIX_SUBSCRIPTION
#define _NIX_SUBSCRIPTION

#include "Subscriber.h"

class Subscription
{
public:
	Subscription(Subscriber* subscriber, char type);
	~Subscription();

	char getType();
	Subscriber* getSubscriber();

private:
	char type;
	Subscriber* subscriber;
};

#endif