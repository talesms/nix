#include "DataBase.h"
#include <string>


int main()
{
	DataBase dataBase;
	vector<Avatar*>* avatarList;
	int userid;
	Avatar* lastAvatar;

	if (userid = dataBase.login(string("testuser"), string("test")))
	{
		std::cout << "SUCCESS testuser has loged in" << std::endl;

		avatarList = dataBase.getCharacterList(userid);

		for(std::vector<Avatar*>::iterator it = avatarList->begin();
			it!= avatarList->end(); ++it)
		{
			std::cout << "id: " << (*it)->idavatar << std::endl;
			std::cout << "name: " << (*it)->name << std::endl;
			std::cout << "level: " << (*it)->level << std::endl;
			std::cout << "sex: " << (*it)->sex << std::endl;
			std::cout << "maxhp: " << (*it)->maxhp << std::endl;
			std::cout << "hp: " << (*it)->hp << std::endl;
			std::cout << "maxmana: " << (*it)->maxmana << std::endl;
			std::cout << "mana: " << (*it)->mana << std::endl;
			std::cout << "positionx: " << (*it)->positionx << std::endl;
			std::cout << "positiony: " << (*it)->positiony << std::endl;
			std::cout << "positionz: " << (*it)->positionz << std::endl;
			std::cout << "rotationx: " << (*it)->rotationx << std::endl;
			std::cout << "rotationy: " << (*it)->rotationy << std::endl;
			std::cout << "rotationz: " << (*it)->rotationz << std::endl;
			std::cout << "================================= " << std::endl;
		}

		lastAvatar = (*avatarList)[1];
		lastAvatar->level = 2;
		lastAvatar->maxhp = 200;

		dataBase.saveCharacter(lastAvatar);
		std::cout << " ------------- Avatar Updated! ------------- " << std::endl;

		avatarList = dataBase.getCharacterList(userid);

		for(std::vector<Avatar*>::iterator it = avatarList->begin();
			it!= avatarList->end(); ++it)
		{
			std::cout << "id: " << (*it)->idavatar << std::endl;
			std::cout << "name: " << (*it)->name << std::endl;
			std::cout << "level: " << (*it)->level << std::endl;
			std::cout << "sex: " << (*it)->sex << std::endl;
			std::cout << "maxhp: " << (*it)->maxhp << std::endl;
			std::cout << "hp: " << (*it)->hp << std::endl;
			std::cout << "maxmana: " << (*it)->maxmana << std::endl;
			std::cout << "mana: " << (*it)->mana << std::endl;
			std::cout << "positionx: " << (*it)->positionx << std::endl;
			std::cout << "positiony: " << (*it)->positiony << std::endl;
			std::cout << "positionz: " << (*it)->positionz << std::endl;
			std::cout << "rotationx: " << (*it)->rotationx << std::endl;
			std::cout << "rotationy: " << (*it)->rotationy << std::endl;
			std::cout << "rotationz: " << (*it)->rotationz << std::endl;
			std::cout << "================================= " << std::endl;
		}

		
	}
	else
	{
		std::cout << "Nao Existe usuario!" << std::endl;
	}

	std::cout << "Fim do programa..." << std::endl;

}
