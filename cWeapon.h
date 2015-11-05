#pragma once

#include "cBicho.h"
#include "cData.h"

class cWeapon : public cBicho {

public:
	cWeapon(void);
	~cWeapon(void);

	void Init();
	void SetThrown(bool t);
	bool GetThrown();
	void SetSpeed(int s);
	int GetSpeed();
	void Logic(worldMatrix *map, cRect *playerHitbox);
	void Draw(int tex_id, int enemy);

private:
	bool thrown;
	int speed;
	int next = 0;
};