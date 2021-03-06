#include "cOctorok.h"

cOctorok::cOctorok(void)
{
	SetFrameDelay(8);
	SetWidthHeight(16, 16);
	SetState(rand() % 4);
	SetLives(1.0);
	SetStepLength(rand() % 2 + 1);
	steps = 0;
	Rock.SetWidthHeight(16, 16);
	Rock.SetThrown(false);
	Rock.SetSpeed(3);
	SetHitbox(2, 14, 2, 14);
	Rock.SetHitbox(5, 11, 4, 12);
}

cOctorok::~cOctorok(void)
{
}

void cOctorok::GetRockInfo(int * x, int * y, int *s)
{
	Rock.GetPosition(x, y);
	*s = Rock.GetState();
}

void cOctorok::Logic(worldMatrix * map, cRect *playerHitbox, cRect *swordHitbox, cRect *directSwordHitbox, bool swordThrown, bool directAttack, int px, int py, bool underSpell)
{
	if (!GetWeaponHit()) Rock.SetHit(false);
	if (!GetWeaponThrown()) Rock.SetThrown(false);

	if (!underSpell) {
		if (steps <= 0) {
			steps = rand() % 50;
			SetState(rand() % 8);
		}
		switch (GetState()) {
			case STATE_WALKLEFT:
				MoveLeft(map);
				break;

			case STATE_WALKRIGHT:
				MoveRight(map);
				break;

			case STATE_WALKUP:
				MoveUp(map);
				break;

			case STATE_WALKDOWN:
				MoveDown(map);
				break;

			default: if (!GetWeaponThrown()) SetWeaponThrown(rand() % 5 == 0);
		}

		int x, y;
		GetPosition(&x, &y);
		int state = GetState();
		if (!GetWeaponThrown()) {
			if ((y == py && (state % 4 == 0 && px > x) || (state % 4 == 1 && px < x))
				|| (x == px) && (state % 4 == 2 && py > y) || (state % 4 == 3 && py < y))
				SetWeaponThrown(true);
		}

		steps -= 1;
	}

	if (GetWeaponThrown()) {
		if (!Rock.GetThrown()) {
			int x, y;
			GetPosition(&x, &y);
			Rock.SetPosition(x, y);
			Rock.SetThrown(true);
			Rock.SetState(GetState());
			Rock.ResetDistance();
		}
		Rock.Logic(map, playerHitbox);
		if (!Rock.GetThrown()) SetWeaponThrown(false);
	}

	if (Rock.GetHit()) {
		SetWeaponHit(true);
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

void cOctorok::Draw(int tex_id)
{
	float xo, yo, xf, yf;

	switch (GetState())
	{
		case STATE_LOOKLEFT:		
		case STATE_WALKLEFT:	xo = 0.0f / 128.0f + (GetFrame()*16.0f / 128.0f);	yo = 16.0f / 128.0f;
								NextFrame(2);
								break;	
		case STATE_LOOKRIGHT:
		case STATE_WALKRIGHT:	xo = 64.0f / 128.0f + (GetFrame()*16.0f / 128.0f);	yo = 16.0f / 128.0f;
								NextFrame(2);
								break;

		case STATE_LOOKUP:
		case STATE_WALKUP:		xo = 96.0f / 128.0f + (GetFrame()*16.0f / 128.0f);	yo = 16.0f / 128.0f;
								NextFrame(2);
								break;

		case STATE_LOOKDOWN:	
		case STATE_WALKDOWN:	xo = 32.0f / 128.0f + (GetFrame()*16.0f / 128.0f);	yo = 16.0f / 128.0f;
								NextFrame(2);
								break;

	}
	xf = xo + 16.0f / 128.0f;
	yf = yo - 16.0f / 128.0f;

	DrawRect(tex_id, xo, yo, xf, yf);

	if (Rock.GetThrown()) Rock.Draw(tex_id, OCTOROK);
}
