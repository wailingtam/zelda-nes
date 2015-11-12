#pragma once

#include "cBicho.h"
#include "cWeapon.h"
#include "cBoomerang.h"

#define SPELL_RECHARGE_TIME 60000
#define SPELL_DURATION		7000


class cPlayer: public cBicho
{
public:
	cPlayer();
	~cPlayer();

	void Init();
	void Logic(worldMatrix *map, cRect *playerHitbox);
	void Draw(int tex_id);
	void Attack();
	void Hurt();

	bool GetAttacking();
	bool GetThrowing();
	bool GetLow();
	void SetLow(bool l);
	cRect GetSwordHitbox();
	cRect GetBoomerangHitbox();
	void SetBoomerangComingBack();

	bool usingSword();
	void SetUsingSword(bool s);
	void countTime();
	void castSpell();
	bool canUseMagic();
	bool GetSpellActive();

	enum TeleportDirection {
		tpUp, tpDown, tpLeft, tpRight
	};


	bool teleport(worldMatrix * map, worldMatrix * otherMap, TeleportDirection tpdir);

	void MoveRight(worldMatrix * map, worldMatrix * otherMap, bool * isOverworld);

	void MoveLeft(worldMatrix * map, worldMatrix * otherMap, bool * isOverworld);

	void MoveUp(worldMatrix * map, worldMatrix * otherMap, bool * isOverworld);

	void MoveDown(worldMatrix * map, worldMatrix * otherMap, bool * isOverworld);


private:
	cWeapon Sword;
	cBoomerang Boomerang;
	bool attacking;
	bool throwing;
	bool low;
	bool weaponIsSword;
	
	bool _canUseMagic = true;
	unsigned int initialTime = glutGet(GLUT_ELAPSED_TIME);
	bool spellActive = false;
};
