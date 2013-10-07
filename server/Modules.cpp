#include "Modules.h"

Modules::Modules()
{
}

Modules::~Modules()
{
}

void Modules::update(Message* message)
{
	int n;
	switch(message->getDestination())
	{
		case REGION:
		n = write(modulesSocks[0], message, sizeof(Message));
		break;
	}
}

void Modules::addModule(int sockModule)
{
	modulesSocks.push_back(sockModule);
}