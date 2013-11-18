#ifndef _NIX_CACHEMANAGER
#define _NIX_CACHEMANAGER

#include "DataBase.h"

#include "../core/Configuration.h"
#include "../core/MessageConfig.h"
#include "../core/LoginMessage.h"
#include "../core/CharacterListMessage.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#define SIZE_AUTORIZATION_KEY 14

class CacheManager
{
public:
	CacheManager();
	~CacheManager();

	void run(const char* authotizationKey);

private:

	DataBase database;
	bool checkKey(const char* originalKey, char* key);
	char authotizationKey[SIZE_AUTORIZATION_KEY];
};

#endif