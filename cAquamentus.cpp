#include "cAquamentus.h"

cAquamentus::cAquamentus(void)
{
}

cAquamentus::~cAquamentus(void)
{
}

void cAquamentus::Init()
{
	SetFrameDelay(16);
	SetWidthHeight(32, 32);
	SetTile(30, 2);
	SetState(STATE_WALKLEFT);
	SetStepLength(1);
	SetLives(6.0);
	SetAlive(true);
	SetWeaponThrown(false);
	SetHitbox(8, 22, 1, 22);
	steps = 0;
	for (int i = 0; i < 3; ++i) {
		Fire[i].SetWidthHeight(16, 16);
		Fire[i].SetThrown(false);
		Fire[i].SetSpeed(3);
		Fire[i].SetFrameDelay(1);
		Fire[i].SetHitbox(5, 11, 4, 12);
	}
}

void cAquamentus::Logic(worldMatrix * map, cRect *playerHitbox, cRect *swordHitbox, bool swordThrown)
{

	if (steps == 0) {
		steps = rand() % 25 + 5;
		direction = rand() % 2 + 4;
		SetState(direction);
	}

	switch (direction) {
		case STATE_WALKLEFT:
			MoveLeft(map);
			break;

		case STATE_WALKRIGHT:
			MoveRight(map);
			break;
	}

	if (!GetWeaponThrown()) {
		SetWeaponThrown(rand() % 2);
		if (GetWeaponThrown()) {
			for (int i = 0; i < 3; ++i) {
				int px, py;
				GetPosition(&px, &py);
				Fire[i].SetPosition(px, py);
				Fire[i].SetThrown(true);
				Fire[i].SetState(STATE_WALKLEFT);
			}
		}
	}

	steps -= 1;

	if (GetWeaponThrown()) {
		bool fireOn = false;
		for (int i = 0; i < 3; ++i) {
			if (Fire[i].GetThrown()) {
				int ax, ay;
				Fire[i].GetPosition(&ax, &ay);
				ax -= Fire[i].GetSpeed();
				if (ax % 4 == 0) {
					switch (i) {
					case 0:
						ay += 1;
						break;
					case 2:
						ay -= 1;
					}
				}
				Fire[i].SetPosition(ax, ay);
				Fire[i].Logic(map, playerHitbox);
				fireOn = true;
			}
		}
		
		if (!fireOn) SetWeaponThrown(false);
	}

	SetHit(Collides(playerHitbox));
/*	if (swordThrown) {
		SetHurt(Collides(swordHitbox));
	}*/
}

void cAquamentus::Draw(int tex_id)
{
	float xo, yo, xf, yf;

	xo = 64.0f / 128.0f + (GetFrame()*32.0f / 128.0f);

	if (!GetWeaponThrown()) yo = 96.0f / 128.0f;
	else yo = 128.0f / 128.0f;

	NextFrame(2);

	xf = xo + 32.0f / 128.0f;
	yf = yo - 32.0f / 128.0f;

	DrawRect(tex_id, xo, yo, xf, yf);

	if (GetWeaponThrown()) {
		for (int i = 0; i < 3; ++i) {
			if (Fire[i].GetThrown()) Fire[i].Draw(tex_id, AQUAMENTUS);
		}
	}
}