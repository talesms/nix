#include "DataBase.h"
#include <string>


int main()
{
	DataBase dataBase;
	vector<Avatar*>* avatarList;
	vector<Item*>* itemList;
	vector<Skill*>* skillList;
	int userid;
	Avatar* lastAvatar;

	std::cout << "Getting item list..." << std::endl;
	itemList = dataBase.getItemList();
	for(std::vector<Item*>::iterator it = itemList->begin();
			it!= itemList->end(); ++it)
	{
		std::cout << "===== item =====: " << std::endl;
		std::cout << "item id: " << (*it)->iditem << std::endl;
		std::cout << "item type: " << (*it)->type << std::endl;
		std::cout << "item subtype: " << (*it)->subtype << std::endl;
		std::cout << "item name: " << (*it)->name << std::endl;
		std::cout << "item minLevel: " << (*it)->minLevel << std::endl;
		std::cout << "item armor: " << (*it)->armor << std::endl;
		std::cout << "item strength: " << (*it)->strength << std::endl;
		std::cout << "item stamina: " << (*it)->stamina << std::endl;
		std::cout << "item intelect: " << (*it)->intelect << std::endl;
		std::cout << "item agility: " << (*it)->agility << std::endl;
		std::cout << "item movimentSpeed: " << (*it)->movimentSpeed << std::endl;
		std::cout << "item note: " << (*it)->note << std::endl;
	}

	std::cout << "Getting skill list..." << std::endl;
	skillList = dataBase.getSkillList();
	for(std::vector<Skill*>::iterator it = skillList->begin();
			it!= skillList->end(); ++it)
	{
		std::cout << "===== skill =====: " << std::endl;
		std::cout << "skill id: " << (*it)->idskill << std::endl;
		std::cout << "skill name: " << (*it)->name << std::endl;
		std::cout << "skill note: " << (*it)->note << std::endl;
		std::cout << "skill mana: " << (*it)->mana << std::endl;
		std::cout << "skill castTime: " << (*it)->castTime << std::endl;
		std::cout << "skill areaRadius: " << (*it)->areaRadius << std::endl;
		std::cout << "skill BaseDamage: " << (*it)->BaseDamage << std::endl;
		std::cout << "skill BaseHeal: " << (*it)->BaseHeal << std::endl;
		std::cout << "skill modifier1: " << (*it)->modifier1 << std::endl;
		std::cout << "skill modifierValue1: " << (*it)->modifierValue1 << std::endl;
		std::cout << "skill modifier2: " << (*it)->modifier2 << std::endl;
		std::cout << "skill modifierValue2: " << (*it)->modifierValue2 << std::endl;
		std::cout << "skill modifier3: " << (*it)->modifier3 << std::endl;
		std::cout << "skill modifierValue3: " << (*it)->modifierValue3 << std::endl;
	}

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
			std::cout << "race: " << (*it)->race << std::endl;
			std::cout << "classNum: " << (*it)->classNum << std::endl;
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
		char c;

		std::cout << " Press enter to update avatar " << lastAvatar->name << std::endl;
		std::cin >> c;

		dataBase.saveCharacter(lastAvatar);
		std::cout << " ------------- Avatar Updated! ------------- " << std::endl;

		avatarList = dataBase.getCharacterList(userid);

		for(std::vector<Avatar*>::iterator it = avatarList->begin();
			it!= avatarList->end(); ++it)
		{
			std::cout << "id: " << (*it)->idavatar << std::endl;
			std::cout << "name: " << (*it)->name << std::endl;
			std::cout << "level: " << (*it)->level << std::endl;
			std::cout << "race: " << (*it)->race << std::endl;
			std::cout << "classNum: " << (*it)->classNum << std::endl;
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
