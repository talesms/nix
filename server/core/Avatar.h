#ifndef _NIX_AVATAR
#define _NIX_AVATAR

#include <string>
#include <cstring>

typedef struct _Avatar
{
	int idavatar;
	char* name;
	int level;
	char sex;
	int maxhp;
	int hp;
	int maxmana;
	int mana;
	int positionx;
	int positiony;
	int positionz;
	int rotationx;
	int rotationy;
	int rotationz;
} Avatar;

#endif
