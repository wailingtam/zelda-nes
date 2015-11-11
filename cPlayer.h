#pragma once

#include "cBicho.h"
#include "cWeapon.h"
#include "cBoomerang.h"

#define SPELL_RECHARGE_TIME 15000



class cPlayer: public cBicho
{
public:
	cPlayer();
	~cPlayer();

	void Init();
	void Logic(worldMatrix *map, cRect *playerHitbox);
	void Draw(int tex_id);
	void Attack(worldMatrix *map);
	void Hurt();
	void countTime();

	bool GetAttacking();
	bool GetThrowing();
	bool GetLow();
	void SetLow(bool l);
	void castSpell();

	bool usingSword();
	bool canUseMagic();

	enum TeleportDirection {
		tpUp, tpDown, tpLeft, tpRight
	};


	bool teleport(worldMatrix * map, worldMatrix * otherMap, TeleportDirection tpdir);

	void MoveRight(worldMatrix * map, worldMatrix * otherMap, bool * isOverworld);

	void MoveLeft(worldMatrix * map, worldMatrix * otherMap, bool * isOverworld);

	void MoveUp(worldMatrix * map, worldMatrix * otherMap, bool * isOverworld);

	void MoveDown(worldMatrix * map, worldMatrix * otherMap, bool * isOverworld);

	cRect GetSwordHitbox();

private:
	cBoomerang Sword;
	cBoomerang Boomerang;
	bool attacking;
	bool throwing;
	bool low; 
	
	bool _canUseMagic = false;
	unsigned int countdown = SPELL_RECHARGE_TIME;
	unsigned int initialTime = glutGet(GLUT_ELAPSED_TIME);
};
