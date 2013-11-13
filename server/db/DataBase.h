#ifndef _NIX_DATABASE
#define _NIX_DATABASE

#include "../core/Avatar.h"
#include "../core/Item.h"
#include "../core/Skill.h"
#include "../core/Configuration.h"

#include <mysql++.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

class DataBase
{
public:
	DataBase();
	~DataBase();

	int login(string user, string password);
	vector<Avatar*>* getCharacterList(int userid);
	vector<Item*>* getItemList();
	vector<Skill*>* getSkillList();
	bool saveCharacter(Avatar* avatar);

private:
	char* saveQuery;
	mysqlpp::Connection *conn;
	mysqlpp::StoreQueryResult::const_iterator it;

};

#endif