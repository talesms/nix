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
		if(!result.empty())
		{
			it = result.begin();
			mysqlpp::Row row = *it;
			return atoi(row[0]);
		}
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
			sprintf(avatar->name, "%s", row[1].c_str());
			avatar->level = atoi(row[2]);
			avatar->race = atoi(row[3]);
			avatar->classNum = atoi(row[4]);
			avatar->sex = row[5].c_str()[0];
			avatar->maxhp = atoi(row[6]);
			avatar->hp = atoi(row[7]);
			avatar->maxmana = atoi(row[8]);
			avatar->mana = atoi(row[9]);
			avatar->positionx = atoi(row[10]);
			avatar->positiony = atoi(row[11]);
			avatar->positionz = atoi(row[12]);
			avatar->rotationx = atoi(row[13]);
			avatar->rotationy = atoi(row[14]);
			avatar->rotationz = atoi(row[15]);

			list->push_back(avatar);
		} 
	}
	return list;
}

vector<Item*>* DataBase::getItemList()
{
	Item* item;
	vector<Item*>* list = new vector<Item*>();
	mysqlpp::Query query = conn->query("SELECT * FROM item");

	if(mysqlpp::StoreQueryResult result = query.store())
	{
		for(it = result.begin(); it != result.end(); ++it)
		{
			mysqlpp::Row row = *it;
			item = (Item*) malloc(sizeof(Item));
			item->iditem = atoi(row[0]);
			item->type = atoi(row[1]);
			item->subtype = atoi(row[2]);
			sprintf(item->name, "%s", row[3].c_str());
			item->minLevel = atoi(row[4]);
			item->armor = atoi(row[5]);
			item->strength = atoi(row[6]);
			item->stamina = atoi(row[7]);
			item->intelect = atoi(row[8]);
			item->agility = atoi(row[9]);
			item->hit = atoi(row[10]);
			item->movimentSpeed = atoi(row[11]);
			sprintf(item->note, "%s",row[12].c_str());

			list->push_back(item);
		} 
	}
	return list;
}

vector<Skill*>* DataBase::getSkillList()
{
	Skill* skill;
	vector<Skill*>* list = new vector<Skill*>();
	mysqlpp::Query query = conn->query("SELECT * FROM skill");

	if(mysqlpp::StoreQueryResult result = query.store())
	{
		for(it = result.begin(); it != result.end(); ++it)
		{
			mysqlpp::Row row = *it;
			skill = (Skill*) malloc(sizeof(Skill));
			skill->idskill = atoi(row[0]);
			sprintf(skill->name, "%s", row[1].c_str());
			sprintf(skill->note, "%s", row[2].c_str());
			skill->mana = atoi(row[3]);
			skill->castTime = atoi(row[4]);
			skill->areaRadius = atoi(row[5]);
			skill->BaseDamage = atoi(row[6]);
			skill->BaseHeal = atoi(row[7]);
			skill->modifier1 = atoi(row[8]);
			skill->modifierValue1 = atof(row[9]);
			skill->modifier2 = atoi(row[10]);
			skill->modifierValue2 = atof(row[11]);
			skill->modifier3 = atoi(row[12]);
			skill->modifierValue3 = atof(row[13]);

			list->push_back(skill);
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

