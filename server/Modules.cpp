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
		case REGION:
		write(modulesSocks[REGION], message, sizeof(Message));
		std::cout << "DEBUG enviando msg para region1" << std::endl;
		break;
	}
}

void Module::addModule(int sockModule)
{
	modulesSocks->push_back(sock);
}