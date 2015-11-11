#include "cBoomerang.h"

cBoomerang::cBoomerang(void)
{
}

cBoomerang::~cBoomerang(void)
{
}


void cBoomerang::SetObjective(int ox, int oy)
{
	obj_x = ox;
	obj_y = oy;
}

void cBoomerang::Logic(worldMatrix *map, cRect *playerHitbox)
{
	int x, y;
	GetPosition(&x, &y);
	bool thrown = GetThrown();
	int speed = GetSpeed();
	if (thrown) {
		if (distance < 200) {
			switch (GetState()) {
			case STATE_LOOKLEFT:
			case STATE_WALKLEFT:	x -= speed;
				if (CollidesMapLimits(map)) thrown = false;
				break;
			case STATE_LOOKRIGHT:
			case STATE_WALKRIGHT:	x += speed;
				if (CollidesMapLimits(map)) thrown = false;
				break;
			case STATE_LOOKUP:
			case STATE_WALKUP:		y += speed;
				if (CollidesMapLimits(map)) thrown = false;
				break;
			case STATE_LOOKDOWN:
			case STATE_WALKDOWN:	y -= speed;
				if (CollidesMapLimits(map)) thrown = false;
				break;
			}
			distance += speed;
			if (distance > 200) thrown = false;
		}
		else {
			float vx = obj_x - x;
			float vy = obj_y - y;
			float mod = sqrt(pow(vx, 2) + pow(vy,2));
			vx = vx*speed / mod;
			vy = vy*speed / mod;
			x = x + floor(vx + 0.5);
			y = y + floor(vy + 0.5);
			SetPosition(x, y);
		}
	}
	SetPosition(x, y);

	SetHit(Collides(playerHitbox));
}

void cBoomerang::Draw(int tex_id, int enemy)
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

	DrawRect(tex_id, xo, yo, xf, yf);
}
