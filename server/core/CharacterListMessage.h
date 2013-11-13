#ifndef _NIX_CHARACTERLISTMESSAGE
#define _NIX_CHARACTERLISTMESSAGE

#include "Avatar.h"

#include <stdlib.h>

class CharacterListMessage
{
public:
	CharacterListMessage(Avatar* list, char listSize);
	~CharacterListMessage();

	int getClientSocket();
	Avatar getCharacter(char index);
	Avatar* getCharacterList();

private:
	int clientSocket;
	Avatar characterList[10];
};

#endif