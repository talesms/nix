#ifndef _NIX_MODULES
#define _NIX_MODULES

#include <vector>
#include <sys/types.h>
#include <socket.h>

#include <iostream> //TODO: apagar, debug

#include "/core/Subscriber.h"

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