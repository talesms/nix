#include "CacheManager.h"

struct DbAccessThreadParam
{
	int sock;
	DataBase* database;
};

CacheManager::CacheManager()
{
	portno = atoi(Configuration::getConfig("cache_manager_port").c_str());
	if(portno < 1 || portno > 65535)
		std::cout << "ERROR invalid cache manager portno." << std::endl;
}

CacheManager::~CacheManager()
{

}

void CacheManager::run()
{
	int sock;
	int newsock;
	int n;
	socklen_t clilen;
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	pthread_t* newThread;
	DbAccessThreadParam* dbAccessThreadParam;
	char buffer[SIZE_AUTORIZATION_KEY];
	char key[SIZE_AUTORIZATION_KEY];

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
		newsockfd = accept(sock, 
			(struct sockaddr *) &cli_addr, &clilen);

		if (newsock < 0)
		{
			std::cout << "ERROR on accept" << std::endl;
			continue;
		}

		n = read(newsock, buffer, SIZE_AUTORIZATION_KEY);
	  	if(!n)
	    	continue;

		if(checkKey(authotizationKey, buffer))
		{
			newThread = new pthread_t();
			dbAccessThreadParam = (DbAccessThreadParam*) calloc(sizeof(DbAccessThreadParam));
			dbAccessThreadParam->sock = newsock;
			dbAccessThreadParam->database = &database;

	  		pthread_create(newThread, NULL, dbAccessThreadFunction, (void*) dbAccessThreadParam);
	  	}


	}
}

void *dbAccessThreadFunction(void* param)
{
	int sock = ((DbAccessThreadParam*)dbAccessThreadParam)->sock;
	Database* database = ((DbAccessThreadParam*)dbAccessThreadParam)->dataBase;
	delete ((DbAccessThreadParam*)param);
	LoginMessage* loginMsg = new LoginMessage(0, " ", " ");
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
		return (void*)-1;

	vectorAvatarList = dataBase->getCharacterList(userid);

	for(int i=0, std::vector<Avatar*>::iterator it = avatarList->begin();
			it!= avatarList->end(); ++it, ++i)
	{
		avatars[i] = *(*it); //TODO: parei aqui 28/10 02:42
	}
}

bool BaseNet::checkKey(char* originalKey, char* key)
{
	for(int i=0; i<SIZE_AUTORIZATION_KEY; i++)
	{
	  if(originalKey[i] != key[i])
	   	return false;
	}

	return true;
}