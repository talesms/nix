#ifndef _NIX_AVATAR
#define _NIX_AVATAR


typedef struct _Avatar
{
	int idavatar;
	char name[12];
	int level;
	int race;
	int classNum;
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
