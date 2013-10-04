#ifndef _NIX_REGIONMESSAGEDELIVERY
#define _NIX_REGIONMESSAGEDELIVERY

#include "../core/Subscriber.h"
#include "../core/Message.h"
#include "../core/Subscription.h"
#include "../core/Publisher.h"

class RegionMessageDelivery : public Subscriber
{
public:
	RegionMessageDelivery();
	~RegionMessageDelivery();

	void update(Message* message);
	void subscribeModule(Subscription* subscription);
	void unsubscribeModule(Subscription* subscription);

private:
	Publisher publisher;
};

#endif