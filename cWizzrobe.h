#pragma once

#include "cBicho.h"
#include "cWeapon.h"

class cWizzrobe : public cBicho {

public:
	cWizzrobe(void);
	~cWizzrobe(void);

	void Logic(worldMatrix *map, int px, int py, cRect *playerHitbox, cRect *swordHitbox, cRect *directSwordHitbox, bool swordThrown, bool directAttack, bool underSpell);
	void Draw(int tex_id);

	void NextAppearance(int nextAppearanceTime);
	int GetAppearanceTime();

private:
	cWeapon Ray;
	int appearanceTime;
	int nextAppearanceTime;
	int disappearanceTime;
	int delay2;
	bool disappeared;
};