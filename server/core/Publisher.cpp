#include "Publisher.h"

Publisher::Publisher()
{
}

Publisher::~Publisher()
{
}

void Publisher::subscribe(Subscription* subscription)
{
	if(subscription != 0)
		subscriptions.push_back(subscription);
}

void Publisher::unsubscribe(Subscription* subscription)
{
	Subscription* temp;
	for(int i = 0; i<subscriptions.size(); i++)
	{
		temp = subscriptions[i];
		if(temp == subscription)
			subscriptions.erase(subscriptions.begin() + i);
	}
}

void Publisher::notifySubscribers(Message* message)
{
	std::cout << "DEBUG notifySubscribers..." << std::endl;
	for(int i = 0; i<subscriptions.size(); i++)
	{
		if(subscriptions[i]->getType() == message->getDestination())
			subscriptions[i]->getSubscriber()->update(message);
	}
}

void Publisher::notifySubscribersByOptions(Message* message)
{
	for(int i = 0; i<subscriptions.size(); i++)
	{
		if(subscriptions[i]->getType() == message->getOption())
			subscriptions[i]->getSubscriber()->update(message);
	}
}