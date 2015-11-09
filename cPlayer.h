#pragma once

#include "cBicho.h"
#include "cWeapon.h"

#define PLAYER_START_CX		3
#define PLAYER_START_CY		2
#define SPELL_RECHARGE_TIME 3

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

	bool canUseMagic();

	cRect GetSwordHitbox();

private:
	cWeapon Sword;
	bool attacking;
	bool throwing;
	bool low; 
	
	bool _canUseMagic = false;
	unsigned int countdown = SPELL_RECHARGE_TIME;
	unsigned int initialTime = glutGet(GLUT_ELAPSED_TIME);
};
