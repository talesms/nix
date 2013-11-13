#include "CacheManager.h"


void *dbAccessThreadFunction(void* dbAccessThreadParam);
void *dbCachedInfoThreadFunction(void* dbCachedInfoThreadQueryParam);


struct DbAccessThreadParam
{
	int sock;
	DataBase* database;
};

struct DbCachedInfoThreadParam
{
	DataBase* database;
};

struct DbCachedInfo
{
	int infoType;
	int infoId;
};

CacheManager::CacheManager()
{

}

CacheManager::~CacheManager()
{

}

void CacheManager::run(const char* authotizationKey)
{
	int sock;
	int newsock;
	int n;
	int portno;
	socklen_t clilen;
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	pthread_t* newThread;
	pthread_t* cacheInfoThread;
	DbCachedInfoThreadParam* dbCachedInfoThreadParam;
	DbAccessThreadParam* dbAccessThreadParam;
	char buffer[SIZE_AUTORIZATION_KEY];
	char key[SIZE_AUTORIZATION_KEY];

	cacheInfoThread = new pthread_t();
	dbCachedInfoThreadParam = (DbCachedInfoThreadParam*) malloc(sizeof(DbCachedInfoThreadParam));
	dbCachedInfoThreadParam->database = &database;
	pthread_create(cacheInfoThread, NULL, dbCachedInfoThreadFunction, (void*) dbCachedInfoThreadParam);

	portno = atoi(Configuration::getConfig("cache_manager_port").c_str());
	if(portno < 1 || portno > 65535)
		std::cout << "ERROR invalid cache manager portno." << std::endl;

	for(int i=0; i<SIZE_AUTORIZATION_KEY; i++)
    	key[i] = Configuration::getConfig("cache_manager_key").c_str()[i];

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
		std::cout << "ERROR opening socket." << std::endl;

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

    if (bind(sock, (struct sockaddr *) &serv_addr,
    	sizeof(serv_addr)) < 0)
    	std::cout << "ERROR on binding. " << errno << std::endl;

    std::cout << "WAITING Messages..." << std::endl;

	while(true)
	{
		listen(sock,100);
		clilen = sizeof(cli_addr);
		newsock = accept(sock, 
			(struct sockaddr *) &cli_addr, &clilen);

		if (newsock < 0)
		{
			std::cout << "ERROR on accept" << std::endl;
			continue;
		}

		n = recv(newsock, buffer, SIZE_AUTORIZATION_KEY, 0);
	  	if(!n)
	    	continue;

		if(checkKey(authotizationKey, buffer))
		{
			newThread = new pthread_t();
			dbAccessThreadParam = (DbAccessThreadParam*) malloc(sizeof(DbAccessThreadParam));
			dbAccessThreadParam->sock = newsock;
			dbAccessThreadParam->database = &database;

	  		pthread_create(newThread, NULL, dbAccessThreadFunction, (void*) dbAccessThreadParam);
	  	}
	}
}

void *dbCachedInfoThreadFunction(void* dbCachedInfoThreadParam)
{
	int sock;
	int newsock;
	int n;
	int portno;
	socklen_t clilen;
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	pthread_t* newThread;
	DbCachedInfo* dbCachedInfo;
	vector<Item*>* itemList;
	vector<Skill*>* skillList;

	itemList = ((DbCachedInfoThreadParam*)dbCachedInfoThreadParam)->database->getItemList();
	skillList = ((DbCachedInfoThreadParam*)dbCachedInfoThreadParam)->database->getSkillList();

	dbCachedInfo = (DbCachedInfo*) malloc(sizeof(DbCachedInfo));

	portno = atoi(Configuration::getConfig("cache_manager_cached_port").c_str());
	if(portno < 1 || portno > 65535)
		std::cout << "ERROR invalid cache manager portno at cached thread." << std::endl;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
		std::cout << "ERROR opening socket at cached thread." << std::endl;

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

    if (bind(sock, (struct sockaddr *) &serv_addr,
    	sizeof(serv_addr)) < 0)
    	std::cout << "ERROR on binding at cached thread." << errno << std::endl;

    std::cout << "WAITING Messages at cached thread..." << std::endl;

	while(true)
	{
		listen(sock,100);
		clilen = sizeof(cli_addr);
		newsock = accept(sock, 
			(struct sockaddr *) &cli_addr, &clilen);

		if (newsock < 0)
		{
			std::cout << "ERROR on accept at cached thread." << std::endl;
			continue;
		}

		dbCachedInfo = (DbCachedInfo*) malloc(sizeof(DbCachedInfo));

		n = recv(newsock, dbCachedInfo, sizeof(DbCachedInfo), 0);
	  	if(!n)
	    	continue;

	    switch(dbCachedInfo->infoType)
	    {
	    	case 0:
				send(newsock, &(itemList[dbCachedInfo->infoId]), sizeof(Item), 0);
	    	break;
	    	case 1:
				send(newsock, skillList->at(dbCachedInfo->infoId), sizeof(Skill), 0);
	    	break;
	    }

	}
}

void *dbAccessThreadFunction(void* dbAccessThreadParam)
{
	int sock = ((DbAccessThreadParam*)dbAccessThreadParam)->sock;
	DataBase* database = ((DbAccessThreadParam*)dbAccessThreadParam)->database;
	char nullChar = '\0';
	delete (DbAccessThreadParam*)dbAccessThreadParam;
	LoginMessage* loginMsg = new LoginMessage(&nullChar, &nullChar);
	Avatar avatars[10];
	CharacterListMessage* avatarList = new CharacterListMessage(avatars, 0);
	int userid = 0;
	vector<Avatar*>* vectorAvatarList;

	if(recv(sock, (char*) loginMsg, sizeof(LoginMessage), 0) <= 0)
	{
		std::cout << "ERROR requester went offline" << std::endl;
		return (void*) -1;
	}

	userid = database->login(loginMsg->getUsername(), loginMsg->getPassword());

	if(userid <= 0)
		return (void*) -1;
	std::cout << "SUCCESS " << loginMsg->getUsername() << " has come online." << std::endl;

	vectorAvatarList = database->getCharacterList(userid);

	int i=0;
	for(std::vector<Avatar*>::iterator it = vectorAvatarList->begin(); it != vectorAvatarList->end(); ++it, ++i)
		avatarList->getCharacterList()[i] = **it;

	send(sock, avatarList, sizeof(CharacterListMessage), 0);

}

bool CacheManager::checkKey(const char* originalKey, char* key)
{
	for(int i=0; i<SIZE_AUTORIZATION_KEY; i++)
	{
	  if(originalKey[i] != key[i])
	   	return false;
	}

	return true;
}