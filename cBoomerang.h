#pragma once


#include "cWeapon.h"
#include "cData.h"

#define PI 3.14
class cBoomerang : public cWeapon {

public:
	cBoomerang(void);
	~cBoomerang(void);

	void SetObjective(int ox, int oy);
	void SetComingBack(bool cb);
	bool GetComingBack();

	void Logic(worldMatrix *map, cRect *playerHitbox);
	void Draw(int tex_id);

private:

	int obj_x, obj_y;
	bool comingBack;

};