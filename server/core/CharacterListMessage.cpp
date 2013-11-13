#include "CharacterListMessage.h"

CharacterListMessage::CharacterListMessage(Avatar* list, char listSize)
{
	char i;

	char* nullChar = (char*) malloc(sizeof(char));
	*nullChar = '\0';

	Avatar nullAvatar;
	nullAvatar.idavatar = 0;
	nullAvatar.name[0] = *nullChar;
	nullAvatar.level = 0;
	nullAvatar.race = 0;
	nullAvatar.classNum = 0;
	nullAvatar.sex = *nullChar;
	nullAvatar.maxhp = 0;
	nullAvatar.hp = 0;
	nullAvatar.maxmana = 0;
	nullAvatar.mana = 0;
	nullAvatar.positionx = 0;
	nullAvatar.positiony = 0;
	nullAvatar.positionz = 0;
	nullAvatar.rotationx = 0;
	nullAvatar.rotationy = 0;
	nullAvatar.rotationz = 0;

	for(i=0; i<10; ++i)
		if(i<listSize)
			characterList[i] = list[i];
		else
			characterList[i] = nullAvatar;
}

CharacterListMessage::~CharacterListMessage()
{

}

int CharacterListMessage::getClientSocket()
{
	return clientSocket;
}

Avatar CharacterListMessage::getCharacter(char index)
{
	return characterList[index];
}

Avatar* CharacterListMessage::getCharacterList()
{
	return characterList;
}
