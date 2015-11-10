#pragma once

#include "cBicho.h"

class cTektike : public cBicho {

public:
	cTektike(void);
	~cTektike(void);

	void Jump(worldMatrix *map);
	void Logic(worldMatrix *map, cRect *playerHitbox, cRect *swordHitbox, cRect *directSwordHitbox, bool swordThrown, bool directAttack);
	void Draw(int tex_id);

private:
	int tx, ty;
	bool jumping;
	int jump_alfa;
	int jump_y, jump_x;
	int jump_freq;
	int jump_delay;
	int jump_x_distance;
	int jump_up;
	int jump_alfa_limit;
};