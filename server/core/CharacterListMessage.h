#ifndef _NIX_CHARACTERLISTMESSAGE
#define _NIX_CHARACTERLISTMESSAGE

#include "Avatar.h"

class CharacterListMessage
{
public:
	CharacterListMessage(Avatar* list, int listSize);
	~CharacterListMessage();

	int getClientSocket();
	Avatar getCharacter(int index);
	Avatar* getCharacterList();

private:
	int clientSocket;
	Avatar characterList[10];
};

#endif