#pragma once

#include "cBicho.h"
#include "cWeapon.h"

class cAquamentus : public cBicho {

public:
	cAquamentus(void);
	~cAquamentus(void);

	void Init();
	void Logic(worldMatrix *map, cRect *playerHitbox, cRect *swordHitbox, cRect *directSwordHitbox, bool swordThrown, bool directAttack, bool underSpell);
	void Draw(int tex_id);

private:
	cWeapon Fire[3];
	int steps;
	int fireUnits;
	int fireHitId;
	bool fireHit;
};