#pragma once

#include "cBicho.h"
#include "cWeapon.h"

class cOctorok : public cBicho {

public:
	cOctorok(void);
	~cOctorok(void);

	void Logic(worldMatrix *map, cRect *playerHitbox, cRect *swordHitbox, cRect *directSwordHitbox, bool swordThrown, bool directAttack);
	void Draw(int tex_id);

private:
	cWeapon Rock;
	int steps;
	int direction;
};