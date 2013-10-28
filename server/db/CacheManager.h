#ifndef _NIX_CACHEMANAGER
#define _NIX_CACHEMANAGER

#include "../core/Configuration.h"
#include "../core/MessageConfig.h"
#include "../core/LoginMessage.h"
#include "../core/CharacterListMessage.h"

#define SIZE_AUTORIZATION_KEY 14


class CacheManager
{
public:
	CacheManager();
	~CacheManager();

	void run();

private:
	DataBase database;
	int portno;
	bool checkKey(char* originalKey, char* key);
};

#endif