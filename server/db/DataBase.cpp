#include "DataBase.h"

DataBase::DataBase()
{
	saveQuery = (char*) malloc(sizeof(char)*305);
        conn = new mysqlpp::Connection(false);
	if(conn->connect("nix", Configuration::getConfig("db_ip").c_str(),
		Configuration::getConfig("db_user").c_str(),
		Configuration::getConfig("db_password").c_str(),
		atoi(Configuration::getConfig("db_port").c_str())))
	{
		std::cout << "SUCCESS Connection Successful with mysql database" << std::endl;
	}
	else
	{
		std::cout << "ERROR Connection with mysql database failed " << std::endl;
	}
}

DataBase::~DataBase()
{
	delete conn;
}

int DataBase::login(string user, string password)
{
	mysqlpp::Query query = conn->query("SELECT iduser FROM user WHERE username='" 
			+ user + "' and password='" + password + "'");

	if(mysqlpp::StoreQueryResult result = query.store())
	{
		it = result.begin();
		mysqlpp::Row row = *it;
		return atoi(row[0]);
	}

	return 0;
}

vector<Avatar*>* DataBase::getCharacterList(int userid)
{
	Avatar* avatar;
	char* id;

	sprintf(id, "%d", userid);

	vector<Avatar*>* list = new vector<Avatar*>();
	mysqlpp::Query query = conn->query("SELECT * FROM avatar WHERE owner='" 
		+ string(id) + "'");

	if(mysqlpp::StoreQueryResult result = query.store())
	{
		for(it = result.begin(); it != result.end(); ++it)
		{
			mysqlpp::Row row = *it;
			avatar = (Avatar*) malloc(sizeof(Avatar));
			avatar->idavatar = atoi(row[0]);
			avatar->name = (char*) malloc(sizeof(row[1].length()));
			sprintf(avatar->name, "%s", row[1].c_str());
			avatar->level = atoi(row[2]);
			avatar->sex = row[3].c_str()[0];
			avatar->maxhp = atoi(row[4]);
			avatar->hp = atoi(row[5]);
			avatar->maxmana = atoi(row[6]);
			avatar->mana = atoi(row[7]);
			avatar->positionx = atoi(row[8]);
			avatar->positiony = atoi(row[9]);
			avatar->positionz = atoi(row[10]);
			avatar->rotationx = atoi(row[11]);
			avatar->rotationy = atoi(row[12]);
			avatar->rotationz = atoi(row[13]);

			list->push_back(avatar);
		} 
	}
	return list;
}

bool DataBase::saveCharacter(Avatar* avatar)
{
	sprintf(saveQuery, "UPDATE `nix`.`avatar` SET `level`=%d, `maxhp`='%d', `hp`='%d', `maxmana`='%d', `mana`='%d', `positionx`=%d, `positiony`=%d, `positionz`=%d, `rotationx`=%d, `rotationy`=%d, `rotationz`=%d WHERE `idavatar`='%d'", avatar->level, avatar->maxhp, avatar->hp, avatar->maxmana, avatar->mana, avatar->positionx, avatar->positiony, avatar->positionz, avatar->rotationx, avatar->rotationy, avatar->rotationz, avatar->idavatar);

	mysqlpp::Query query = conn->query(saveQuery);
	query.store();

	
}










