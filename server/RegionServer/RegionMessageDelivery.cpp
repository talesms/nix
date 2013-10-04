#include "RegionMessageDelivery.h"

RegionMessageDelivery::RegionMessageDelivery()
{
}

RegionMessageDelivery::~RegionMessageDelivery()
{
}

void RegionMessageDelivery::update(Message* message)
{
	publisher.notifySubscribersByOptions(message);
}

void RegionMessageDelivery::subscribeModule(Subscription* subscription)
{
	publisher.subscribe(subscription);
}

void RegionMessageDelivery::unsubscribeModule(Subscription* subscription)
{
	publisher.unsubscribe(subscription);
}