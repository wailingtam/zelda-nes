#include "cTektike.h"

cTektike::cTektike(void)
{
}

cTektike::~cTektike(void)
{
}

void cTektike::Init()
{
	SetFrameDelay(16);
	SetWidthHeight(16, 16);
	SetTile(10, 20);
	SetStepLength(3);
	SetLives(1.0);
	SetAlive(true);
	SetHitbox(1, 15, 1, 14);
	jumping = false;
	jump_delay = 0;
	jump_freq = rand() % 240 + 250;
}

void cTektike::Jump(worldMatrix * map)
{
	if (!jumping)
	{
		//if (CollidesMapFloor(map))
		//{
			jumping = true;
			jump_alfa = 0;
			GetPosition(&tx, &ty);
			jump_y = ty;
			jump_x = tx;
			jump_x_distance = rand() % 100 + 30;
			jump_up = rand() % 2;
			jump_alfa_limit = (rand() % 35 + 10) * 4;
			SetState(rand() % 2 + 4);
		//}
	}
}

void cTektike::Logic(worldMatrix * map, cRect *playerHitbox, cRect *swordHitbox, bool swordThrown)
{
	jump_delay++;
	if (jump_delay == jump_freq) {
		jump_delay = 0;
		Jump(map);
	}

	float alfa;

	if(jumping)
	{
		jump_alfa += JUMP_STEP;
		
		if(jump_alfa == jump_alfa_limit)
		{
			jumping = false;
			//ty = jump_y;
			//SetPosition(tx, ty);
		}
		else
		{
			alfa = ((float)jump_alfa) * 0.017453f;
			if (jump_up) ty = jump_y + (int)( ((float)JUMP_HEIGHT) * sin(alfa) );
			else  ty = jump_y + (int)(((float)JUMP_HEIGHT) * cos(alfa)) - JUMP_HEIGHT;
			if (jump_x_distance > 0) {
				if (GetState() == STATE_WALKRIGHT) jump_x += GetStepLength();
				else jump_x -= GetStepLength();
				jump_x_distance -= GetStepLength();
			}
			tx = jump_x;
			SetPosition(tx, ty);

			/*if(jump_alfa > 90)
			{
				//Over floor?
				jumping = !CollidesMapFloor(map);
			}*/
		}
	}
	/*else
	{
		//Over floor?
		if(!CollidesMapFloor(map))
			ty -= (2*GetStepLength());
			SetPosition(tx, ty);
	}*/
	SetHit(Collides(playerHitbox));
/*	if (swordThrown) {
		SetHurt(Collides(swordHitbox));
	}*/
}

void cTektike::Draw(int tex_id)
{
	float xo, yo, xf, yf;

	if (!jumping) {
		xo = 0.0f / 128.0f + (GetFrame()*16.0f / 128.0f);	yo = 32.0f / 128.0f;
		NextFrame(2);
	}
	else {
		xo = 16.0f / 128.0f;	yo = 32.0f / 128.0f;
	}

	xf = xo + 16.0f / 128.0f;
	yf = yo - 16.0f / 128.0f;

	DrawRect(tex_id, xo, yo, xf, yf);
}
