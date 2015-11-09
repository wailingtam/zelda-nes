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
	SetTile(30, 15);
	SetState(STATE_WALKLEFT);
	SetStepLength(1);
	SetLives(6.0);
	SetHitbox(7, 26, 1, 29);
	steps = 0;
	for (int i = 0; i < 3; ++i) {
		Fire[i].SetWidthHeight(16, 16);
		Fire[i].SetThrown(false);
		Fire[i].SetSpeed(2);
		Fire[i].SetFrameDelay(1);
		Fire[i].SetHitbox(5, 11, 4, 12);
	}
	fireHitId = -1;
}

void cAquamentus::Logic(worldMatrix * map, cRect *playerHitbox, cRect *swordHitbox, cRect *directSwordHitbox, bool swordThrown, bool directAttack)
{
	if (fireHitId != -1) {
		if (!GetWeaponHit()) {
			Fire[fireHitId].SetThrown(false);
			--fireUnits;
			if (fireUnits == 0) SetWeaponThrown(false);
		}
		else SetWeaponHit(false);
		Fire[fireHitId].SetHit(false);
		fireHitId = -1;
	}

	if (steps == 0) {
		steps = rand() % 25 + 5;
		SetState(rand() % 2 + 4);
	}

	switch (GetState()) {
		case STATE_WALKLEFT:
			MoveLeft(map);
			break;

		case STATE_WALKRIGHT:
			MoveRight(map);
			break;
	}

	if (!GetWeaponThrown()) {
		SetWeaponThrown(rand() % 250 == 0);
		if (GetWeaponThrown()) {
			for (int i = 0; i < 3; ++i) {
				int px, py;
				GetPosition(&px, &py);
				Fire[i].SetPosition(px, py);
				Fire[i].SetThrown(true);
				Fire[i].SetState(STATE_WALKLEFT);
				Fire[i].ResetDistance();
			}
			fireUnits = 3;
		}
	}

	steps -= 1;

	if (GetWeaponThrown()) {
		for (int i = 0; i < 3; ++i) {
			if (Fire[i].GetThrown()) {
				int ax, ay;
				Fire[i].GetPosition(&ax, &ay);
				ax -= Fire[i].GetSpeed();
				if (GetFrame() == 0) {
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
				if (!Fire[i].GetThrown()) {
					--fireUnits;
					if (fireUnits == 0) SetWeaponThrown(false);
				}
				if (Fire[i].GetHit()) {
					fireHitId = i;
					SetWeaponHit(true);
				}
			}
		}
	}

	SetHit(Collides(playerHitbox));

	if (swordThrown) {
		if (Collides(swordHitbox) && !GetImmune()) Hurt();
	}
	else if (directAttack) {
		if (Collides(directSwordHitbox) && !GetImmune())
			Hurt();
	}
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

	if (GetImmune()) {
		int it = GetImmuneTime();
		if (it > 0) {
			if (it % 50 < 25) DrawRect(tex_id, xo, yo, xf, yf);
			SetImmuneTime(--it);
		}
		else SetImmune(false);
	}
	if (!GetImmune()) DrawRect(tex_id, xo, yo, xf, yf);

	if (GetWeaponThrown()) {
		for (int i = 0; i < 3; ++i) {
			if (Fire[i].GetThrown()) Fire[i].Draw(tex_id, AQUAMENTUS);
		}
	}
}