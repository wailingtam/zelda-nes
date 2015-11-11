#pragma once


#include "cWeapon.h"
#include "cData.h"

#define PI 3.14
class cBoomerang : public cWeapon {

public:
	cBoomerang(void);
	~cBoomerang(void);

	void SetObjective(int ox, int oy);

	void Logic(worldMatrix *map, cRect *playerHitbox);
	void Draw(int tex_id, int enemy);

private:

	int obj_x, obj_y;

};