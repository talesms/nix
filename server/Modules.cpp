#include "Modules.h"

Modules::Modules()
{
}

Modules::~Modules()
{
}

void Modules::update(Message* message)
{
	switch(message->getDestination())
	{
		case MESSAGE_DESTINATION_LOGIN:
			write(modulesSocks[0], message, sizeof(Message));
		break;
		case MESSAGE_DESTINATION_REGION:
			write(modulesSocks[1], message, sizeof(Message));
		break;
	}
}

void Modules::addModule(int sockModule)
{
	modulesSocks.push_back(sockModule);
}
