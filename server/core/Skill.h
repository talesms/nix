#ifndef _NIX_SKILL
#define _NIX_SKILL


typedef struct _Skill
{
	int idskill;
	char name[45];
	char note[200];
	int mana;
	int castTime;
	int areaRadius;
	int BaseDamage;
	int BaseHeal;
	int modifier1;
	float modifierValue1;
	int modifier2;
	float modifierValue2;
	int modifier3;
	float modifierValue3;
} Skill;

#endif
