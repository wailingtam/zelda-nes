#include "cExplosion.h"



cExplosion::cExplosion()
{
	SetWidthHeight(16, 16);
}


cExplosion::~cExplosion()
{
}

void cExplosion::expand (int part)
{
	int x, y;
	GetPosition(&x, &y);
	switch(part) {
		case 0: x -= 1;		y += 1;
				break;
		case 1: x += 1;		y += 1;
				break;
		case 2: x -= 1;		y -= 1;
				break;
		case 3: x += 1;		y -= 1;
	}
	SetPosition(x, y);
}

void cExplosion::draw (int part, int tex_id)
{
	float xo, yo, xf, yf;

	switch (part)
	{
		case 0:	xo = 64.0f / 128.0f;	yo = 32.0f / 128.0f;
				break;
		case 1:	xo = 80.0f / 128.0f;	yo = 32.0f / 128.0f;
				break;

		case 2:	xo = 96.0f / 128.0f;	yo = 32.0f / 128.0f;
				break;

		case 3:	xo = 112.0f / 128.0f;	yo = 32.0f / 128.0f;
				break;
	}
	xf = xo + 16.0f / 128.0f;
	yf = yo - 16.0f / 128.0f;

	DrawRect(tex_id, xo, yo, xf, yf);
}
