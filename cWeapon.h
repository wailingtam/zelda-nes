#pragma once

#include "cBicho.h"
#include "cData.h"

class cWeapon : public cBicho {

public:
	cWeapon(void);
	~cWeapon(void);

	void SetThrown(bool t);
	bool GetThrown();
	void SetSpeed(int s);
	int GetSpeed();
	void ResetDistance();
	cRect GetCurrentHitbox();
	void Logic(worldMatrix *map, cRect *playerHitbox);
	void Draw(int tex_id, int enemy);

protected:
	int distance;

private:
	bool thrown;
	int speed;
	int next;
};