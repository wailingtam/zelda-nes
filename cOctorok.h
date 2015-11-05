#pragma once

#include "cBicho.h"
#include "cWeapon.h"

class cOctorok : public cBicho {

public:
	cOctorok(void);
	~cOctorok(void);

	void Init();
	void Logic(worldMatrix *map, cRect *playerHitbox, cRect *swordHitbox, bool swordThrown);
	void Draw(int tex_id);

private:
	cWeapon Rock;
	int steps;
	int direction;
};