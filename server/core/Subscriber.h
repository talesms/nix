#ifndef _NIX_SUBSCRIBER
#define _NIX_SUBSCRIBER

#include "Message.h"

class Subscriber
{
public:
	Subscriber();
	~Subscriber();

	virtual void update(Message* message) = 0;
};

#endif