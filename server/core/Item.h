#ifndef _NIX_ITEM
#define _NIX_ITEM


typedef struct _Item
{
	int iditem;
	int type;
	int subtype;
	char name[45];
	int minLevel;
	int armor;
	int strength;
	int stamina;
	int intelect;
	int agility;
	int hit;
	float movimentSpeed;
	char note[100];
} Item;

#endif
