#ifndef _NIX_MODULES
#define _NIX_MODULES

#include <vector>
#include <unistd.h>

#include <iostream> //TODO: apagar, debug

#include "core/Subscriber.h"
#include "core/MessageConfig.h"

class Modules : public Subscriber
{
public:
	Modules();
	~Modules();

	void update(Message* message);
	void addModule(int sockModule);

private:
  std::vector<int> modulesSocks;
};

#endif