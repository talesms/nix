#ifndef _NIX_NETWORK
#define _NIX_NETWORK

#include "core/MesageDelivery.h"
#include "core/Message.h"

class Network
{
public:
	Network();
	~Network();

	run(MesageDelivery mesageDelivery);
};

#endif
