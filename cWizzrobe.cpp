#include "cWizzrobe.h"

cWizzrobe::cWizzrobe(void)
{
}

cWizzrobe::~cWizzrobe(void)
{
}

void cWizzrobe::Init()
{
	SetFrameDelay(8);
	SetWidthHeight(16, 16);
	SetTile(15, 2);
	SetState(STATE_LOOKRIGHT);
	SetLives(1.0);
	SetAlive(true);
	SetWeaponThrown(false);
	appearanceTime = 0;
	disappeared = false;
	Ray.SetWidthHeight(16, 16);
	Ray.SetTile(15, 2);
	Ray.SetThrown(false);
	Ray.SetSpeed(2);
	Ray.SetFrameDelay(1);

	SetHitbox(4, 12, 3, 13);
	Ray.SetHitbox(1, 15, 1, 15);
}

void cWizzrobe::Logic(worldMatrix * map, int px, int py, cRect *playerHitbox, cRect *swordHitbox, bool swordThrown)
{

	if (GetAppearanceTime() == 0) {
		int sameRow = rand() % 2;
		int wx, wy;
		if (!sameRow) {
			wx = px + TILE_SIZE;
			wy = rand() % (25 * TILE_SIZE) + 1 * TILE_SIZE;
			if (wy > py) SetState(STATE_LOOKDOWN);
			else SetState(STATE_LOOKUP);
		}
		else {
			wx = rand() % (31 * TILE_SIZE) + 2 * TILE_SIZE;
			wy = py + TILE_SIZE;
			if (wx > px) SetState(STATE_LOOKLEFT);
			else SetState(STATE_LOOKRIGHT);
		}
		SetPosition(wx, wy);
		++appearanceTime;
		nextAppearanceTime = rand() % 100 + 40;
		disappearanceTime = rand() % (nextAppearanceTime - 39) + 41;
		disappeared = false;
	}
	else if (GetAppearanceTime() == 10) SetWeaponThrown(true);

	NextAppearance(nextAppearanceTime);
	
	if (appearanceTime > disappearanceTime 
		|| appearanceTime < 10 && (appearanceTime%3) == 2 
		|| appearanceTime == disappearanceTime - 2 || appearanceTime == disappearanceTime - 4 || appearanceTime == disappearanceTime - 6) 
		disappeared = true;
	else disappeared = false;

	if (GetWeaponThrown()) {
		if (!Ray.GetThrown()) {
			int px, py;
			GetPosition(&px, &py);
			Ray.SetPosition(px, py);
			Ray.SetState(GetState());
			Ray.SetThrown(true);
		}
		Ray.Logic(map, playerHitbox);
		if (!Ray.GetThrown()) SetWeaponThrown(false);
	}

	SetHit(Collides(playerHitbox));
/*	if (swordThrown) {
		SetHurt(Collides(swordHitbox));
	}*/
}

void cWizzrobe::Draw(int tex_id)
{
	if (!disappeared) {

		float xo, yo, xf, yf;

		switch (GetState())
		{
		case STATE_LOOKLEFT:
			xo = 0.0f / 128.0f + (GetFrame()*16.0f / 128.0f);	yo = 64.0f / 128.0f;
			NextFrame(2);
			break;

		case STATE_LOOKRIGHT:
		case STATE_LOOKDOWN:
			xo = 32.0f / 128.0f + (GetFrame()*16.0f / 128.0f);	yo = 64.0f / 128.0f;
			NextFrame(2);
			break;

		case STATE_LOOKUP:
			xo = 64.0f / 128.0f + (GetFrame()*16.0f / 128.0f);	yo = 64.0f / 128.0f;
			NextFrame(2);
			break;
		}

		xf = xo + 16.0f / 128.0f;
		yf = yo - 16.0f / 128.0f;

		DrawRect(tex_id, xo, yo, xf, yf);

	}

	if (Ray.GetThrown()) Ray.Draw(tex_id, WIZZROBE);
}

void cWizzrobe::NextAppearance(int nextAppearanceTime)
{
	delay2++;
	if (delay2 == FRAME_DELAY)
	{	
		appearanceTime++;
		if (appearanceTime == nextAppearanceTime) appearanceTime = 0;
		delay2 = 0;
	}
}

int cWizzrobe::GetAppearanceTime()
{
	return appearanceTime;
}