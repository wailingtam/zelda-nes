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

void cBoomerang::SetComingBack(bool cb)
{
	comingBack = cb;
}

bool cBoomerang::GetComingBack()
{
	return comingBack;
}

void cBoomerang::Logic(worldMatrix *map, cRect *playerHitbox)
{
	int x, y;
	GetPosition(&x, &y);
	bool thrown = GetThrown();
	int speed = GetSpeed();
	if (thrown) {
		if (distance < 200 && !comingBack) {
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
			if (distance >= 200) comingBack = true;
		}
		else if (comingBack) {
			float vx = obj_x - x;
			float vy = obj_y - y;
			if (abs(vx) <= speed && abs(vy) <= speed) {
				SetThrown(false);
				comingBack = false;
				x = obj_x;
				y = obj_y;
			}
			else {
				float mod = sqrt(pow(vx, 2) + pow(vy, 2));
				vx = vx*speed / mod;
				vy = vy*speed / mod;
				x = x + floor(vx + 0.5);
				y = y + floor(vy + 0.5);
			}
		}
	}
	SetPosition(x, y);

	SetHit(Collides(playerHitbox));
}

void cBoomerang::Draw(int tex_id)
{
	float xo, yo, xf, yf;

	xo = 184.0f / 256.0f;
	yo = (GetFrame() + 1) * 46.0f / 256.0f;
	xf = xo + 46.0f / 256.0f;
	yf = yo - 46.0f / 256.0f;
	NextFrame(4);
	DrawRect(tex_id, xo, yo, xf, yf);
}
