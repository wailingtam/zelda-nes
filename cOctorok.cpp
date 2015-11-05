#include "cOctorok.h"

cOctorok::cOctorok(void)
{
}

cOctorok::~cOctorok(void)
{
}

void cOctorok::Init()
{
	SetFrameDelay(8);
	SetWidthHeight(16, 16);
	SetTile(5, 2);
	SetState(STATE_LOOKLEFT);
	SetStepLength(rand() % 3 + 1);
	SetLives(1.0);
	SetAlive(true);
	steps = 0;
	Rock.SetWidthHeight(16, 16);
	Rock.SetTile(10, 2);
	Rock.SetThrown(false);
	Rock.SetState(STATE_LOOKLEFT);
	Rock.SetSpeed(3);
	SetHitbox(4, 12, 4, 12);
	Rock.SetHitbox(5, 11, 4, 12);
}

void cOctorok::Logic(worldMatrix * map, cRect *playerHitbox, cRect *swordHitbox, bool swordThrown)
{
	if (!GetWeaponHit()) Rock.SetHit(false);
	if (!GetWeaponThrown()) Rock.SetThrown(false);

	if (steps == 0) {
		steps = rand() % 100;
		direction = rand() % 8;
		SetState(direction);
	}
	switch (direction) {
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

		default: if(!GetWeaponThrown()) SetWeaponThrown(rand() % 2);
	}
	
	steps -= 1;

	if (GetWeaponThrown()) {
		if (!Rock.GetThrown()) {
			int px, py;
			GetPosition(&px, &py);
			Rock.SetPosition(px, py);
			Rock.SetThrown(true);
			Rock.SetState(GetState());
		}
		Rock.Logic(map, playerHitbox);
		if (!Rock.GetThrown()) SetWeaponThrown(false);
	}

	if (Rock.GetHit()) {
		SetWeaponHit(true);
	}
	else SetHit(Collides(playerHitbox));

	if (swordThrown) {
		if (Collides(swordHitbox) && !GetImmune()) Hurt();
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
