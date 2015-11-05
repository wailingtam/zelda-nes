#include "cWeapon.h"

cWeapon::cWeapon(void)
{
}

cWeapon::~cWeapon(void)
{
}

void cWeapon::Init()
{
}

void cWeapon::SetThrown(bool t)
{
	thrown = t;
}

bool cWeapon::GetThrown()
{
	return thrown;
}

void cWeapon::SetSpeed(int s)
{
	speed = s;
}

int cWeapon::GetSpeed()
{
	return speed;
}

void cWeapon::Logic(worldMatrix *map, cRect *playerHitbox)
{
	int x, y;
	GetPosition(&x, &y);
	if (thrown) {
		switch (GetState()) {
			case STATE_LOOKLEFT:
			case STATE_WALKLEFT:	x -= speed;
									if (CollidesMapWall(map, false)) thrown = false;
									break;
			case STATE_LOOKRIGHT:
			case STATE_WALKRIGHT:	x += speed;
									if (CollidesMapWall(map, true)) thrown = false;
									break;
			case STATE_LOOKUP:
			case STATE_WALKUP:		y += speed;
									if (CollidesMapWall(map, false)) thrown = false;
									break;
			case STATE_LOOKDOWN:
			case STATE_WALKDOWN:	y -= speed;
									if (CollidesMapWall(map, false)) thrown = false;	
									break;
		}
	}
	SetPosition(x, y);

	SetHit(Collides(playerHitbox));
}

void cWeapon::Draw(int tex_id, int enemy)
{
	float xo, yo, xf, yf;

	if (tex_id == IMG_PLAYER) {
		switch (GetState()) {
			case STATE_LOOKLEFT:
			case STATE_WALKLEFT:	
				xo = 46.0f / 256.0f;
				break;
			case STATE_LOOKRIGHT:
			case STATE_WALKRIGHT:	
				xo = 138.0f / 256.0f;
				break;
			case STATE_LOOKUP:
			case STATE_WALKUP:		
				xo = 92.0f / 256.0f;
				break;
			case STATE_LOOKDOWN:
			case STATE_WALKDOWN:	
				xo = 0.0f;
				break;
		}
		
		yo = 230.0f / 256.0f;
		xf = xo + 46.0f / 256.0f;
		yf = yo - 46.0f / 256.0f;
	}
	else if (tex_id == IMG_ENEMIES) {
		if (enemy == OCTOROK) {
			xo = 48.0f / 128.0f;
			yo = 48.0f / 128.0f;
		}
		else if (enemy == WIZZROBE) {
			if (GetFrame() == 0) {
				++next;
				next %= 3;
			}
			switch (GetState()) {
			case STATE_LOOKLEFT:
				xo = 0.0f;
				break;
			case STATE_LOOKRIGHT:
				xo = 32.0f / 128.0f;
				break;
			case STATE_LOOKUP:
				xo = 16.0f / 128.0f;
				break;
			case STATE_LOOKDOWN:
				xo = 48.0f / 128.0f;
				break;
			}
			yo = 80.0f / 128.0f + (next*16.0f / 128.0f);
			NextFrame(15);
		}
		else if (enemy == AQUAMENTUS) {
			if (GetFrame() == 0) {
				++next;
				next %= 4;
			}
			/*switch (GetState()) {
				case STATE_LOOKLEFT:
					xo = 0.0f;
					break;
				case STATE_LOOKRIGHT:
					xo = 32.0f / 128.0f;
					break;
				case STATE_LOOKUP:
					xo = 16.0f / 128.0f;
					break;
				case STATE_LOOKDOWN:
					xo = 48.0f / 128.0f;
					break;
			}
			yo = 80.0f / 128.0f + (next*16.0f / 128.0f);*/
			xo = next*16.0f / 128.0f;
			yo = 128.0f;
			NextFrame(10);
		}

		xf = xo + 16.0f / 128.0f;
		yf = yo - 16.0f / 128.0f;
	}

	DrawRect(tex_id, xo, yo, xf, yf);
}
