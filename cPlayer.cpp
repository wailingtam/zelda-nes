
#include "cPlayer.h"

cPlayer::cPlayer() {}
cPlayer::~cPlayer() {}


void cPlayer::Init()
{
	SetFrameDelay(8);
	SetWidthHeight(46, 46);
	SetTile(2, 1);
	SetState(STATE_LOOKDOWN);
	SetStepLength(2);
	SetLives(3.0);
	SetAlive(true);
	SetWeaponThrown(false);
	Sword.SetWidthHeight(46, 46);
	Sword.SetTile(0, 0);
	Sword.SetThrown(false);
	Sword.SetSpeed(4);
	SetHitbox(16, 28, 16, 30);
	Sword.SetFourHitboxes(16, 30, 22, 23, 0);
	Sword.SetFourHitboxes(16, 30, 22, 23, 1);
	Sword.SetFourHitboxes(23, 24, 15, 29, 2);
	Sword.SetFourHitboxes(23, 24, 15, 29, 3);
}

void cPlayer::Logic(worldMatrix * map, cRect *playerHitbox)
{
	if (!GetWeaponThrown() && Sword.GetThrown()) Sword.SetThrown(false);

	if (GetWeaponThrown()) {
		if (!Sword.GetThrown()) {
			int px, py;
			GetPosition(&px, &py);
			Sword.SetPosition(px, py);
			Sword.SetState(GetState());
			Sword.SetThrown(true);
		}
		Sword.Logic(map, playerHitbox);
		if (!Sword.GetThrown()) SetWeaponThrown(false);
	}
	else Sword.SetThrown(false);
}

void cPlayer::Draw(int tex_id)
{
	float xo, yo, xf, yf;

	switch (GetState())
	{
	case STATE_LOOKLEFT:	if (GetAttackState()) {
		xo = 46.0f / 256.0f;		yo = 184.0f / 256.0f;
		SetAttackState(false);
	}
							else { xo = 46.0f / 256.0f;	yo = 46.0f / 256.0f; }
							break;
	case STATE_LOOKRIGHT:	if (GetAttackState()) {
		xo = 138.0f / 256.0f;		yo = 184.0f / 256.0f;
		SetAttackState(false);
	}
							else { xo = 138.0f / 256.0f;	yo = 46.0f / 256.0f; }
							break;

	case STATE_LOOKUP:		if (GetAttackState()) {
		xo = 92.0f / 256.0f;		yo = 184.0f / 256.0f;
		SetAttackState(false);
	}
							else { xo = 92.0f / 256.0f;			yo = 46.0f / 256.0f; }
							break;

	case STATE_LOOKDOWN:	if (GetAttackState()) {
		xo = 0.0f;					yo = 184.0f / 256.0f;
		SetAttackState(false);
	}
							else { xo = 0.0f;				yo = 46.0f / 256.0f; }
							break;

	case STATE_WALKLEFT:	xo = 46.0f / 256.0f;	yo = 46.0f / 256.0f + (GetFrame()*46.0f / 256.0f);
		NextFrame(2);
		break;

	case STATE_WALKRIGHT:	xo = 138.0f / 256.0f;	yo = 46.0f / 256.0f + (GetFrame()*46.0f / 256.0f);
		NextFrame(2);
		break;

	case STATE_WALKUP:		xo = 92.0f / 256.0f;	yo = 46.0f / 256.0f + (GetFrame()*46.0f / 256.0f);
		NextFrame(2);
		break;

	case STATE_WALKDOWN:	xo = 0.0f;	yo = 46.0f / 256.0f + (GetFrame()*46.0f / 256.0f);
		NextFrame(2);
		break;

	}
	xf = xo + 46.0f / 256.0f;
	yf = yo - 46.0f / 256.0f;

	if (GetImmune()) {
		int it = GetImmuneTime();
		if (it > 0) {
			if (it % 30 < 15) DrawRect(tex_id, xo, yo, xf, yf);
			SetImmuneTime(--it);
		}
		else SetImmune(false);
	}
	if (!GetImmune()) DrawRect(tex_id, xo, yo, xf, yf);

	if (Sword.GetThrown()) Sword.Draw(tex_id, LINK);
}

void cPlayer::Hurt()
{
	SetLives(GetLives() - 0.5);
	if (GetLives() == 0) SetAlive(false);
	else {
		SetImmune(true);
		SetImmuneTime(250);
	}
}

cRect cPlayer::GetSwordHitbox()
{

	int position = Sword.GetState() % 4;
	cRect shb = Sword.vHitbox[position];
	int sx, sy;
	Sword.GetPosition(&sx, &sy);
	shb.left += sx;
	shb.right += sx;
	shb.bottom += sy;
	shb.top += sy;
	return shb;
}

void cPlayer::countTime() {
	unsigned int time = glutGet(GLUT_ELAPSED_TIME);
	if (time - this->initialTime >= countdown)
		this->_canUseMagic = true;
}

void cPlayer::castSpell() {
	if (this->_canUseMagic) {
		this->initialTime = glutGet(GLUT_ELAPSED_TIME);
	}
}

bool cPlayer::canUseMagic() {
	return this->_canUseMagic;
}