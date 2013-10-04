#ifndef _NIX_PUBLISHER
#define _NIX_PUBLISHER

#include <vector>
#include <iostream>

#include "Subscription.h"
#include "Message.h"

class Publisher
{
public:
	Publisher();
	~Publisher();

	void subscribe(Subscription* subscription);
	void unsubscribe(Subscription* subscription);
	void notifySubscribers(Message* message);
	void notifySubscribersByOptions(Message* message);
private:
	std::vector<Subscription*> subscriptions;
};

#endif