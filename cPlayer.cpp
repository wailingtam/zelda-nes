
#include "cPlayer.h"

cPlayer::cPlayer() {}
cPlayer::~cPlayer() {}

void cPlayer::Draw(int tex_id)
{
	float xo, yo, xf, yf;


	switch (GetState())
	{
		//1
	case STATE_LOOKLEFT:	if (GetAttackState()) {
		if (GetLives() == 3) { xo = 40.0f / 256.0f;		yo = 120.0f / 256.0f; }
		else { xo = 40.0f / 256.0f;		yo = 160.0f / 256.0f; }
		SetAttackState(false);
	}
							else { xo = 40.0f / 256.0f;	yo = 40.0f / 256.0f; }
							break;
							//4
	case STATE_LOOKRIGHT:	if (GetAttackState()) {
		if (GetLives() == 3) { xo = 120.0f / 256.0f;		yo = 120.0f / 256.0f; }
		else { xo = 120.0f / 256.0f;		yo = 160.0f / 256.0f; }
		SetAttackState(false);
	}
							else { xo = 120.0f / 256.0f;	yo = 40.0f / 256.0f; }
							break;

	case STATE_LOOKUP:		if (GetAttackState()) {
		if (GetLives() == 3) { xo = 80.0f / 256.0f;		yo = 120.0f / 256.0f; }
		else { xo = 80.0f / 256.0f;		yo = 160.0f / 256.0f; }
		SetAttackState(false);
	}
							else { xo = 80.0f / 256.0f;			yo = 40.0f / 256.0f; }
							break;

	case STATE_LOOKDOWN:	if (GetAttackState()) {
		if (GetLives() == 3) { xo = 0.0f;		yo = 120.0f / 256.0f; }
		else { xo = 0.0f;					yo = 160.0f / 256.0f; }
		SetAttackState(false);
	}
							else { xo = 0.0f;				yo = 40.0f / 256.0f; }
							break;

							//1..3
	case STATE_WALKLEFT:	xo = 40.0f / 256.0f;	yo = 40.0f / 256.0f + (GetFrame()*40.0f / 256.0f);
		NextFrame(2);
		break;
		//4..6
	case STATE_WALKRIGHT:	xo = 120.0f / 256.0f;	yo = 40.0f / 256.0f + (GetFrame()*40.0f / 256.0f);
		NextFrame(2);
		break;

	case STATE_WALKUP:		xo = 80.0f / 256.0f;	yo = 40.0f / 256.0f + (GetFrame()*40.0f / 256.0f);
		NextFrame(2);
		break;

	case STATE_WALKDOWN:	xo = 0.0f;	yo = 40.0f / 256.0f + (GetFrame()*40.0f / 256.0f);
		NextFrame(2);
		break;

	}
	xf = xo + 40.0f / 256.0f;
	yf = yo - 40.0f / 256.0f;

	DrawRect(tex_id, xo, yo, xf, yf);
}
