#include "cBicho.h"
#include "Globals.h"

cBicho::cBicho(void)
{
	seq = 0;
	delay = 0;
	alive = true;
	hit = false;
	weaponThrown = false;
	weaponHit = false;
	hurt = false;
	immune = false;
}
cBicho::~cBicho(void) {}

cBicho::cBicho(int posx, int posy, int width, int height)
{
	x = posx;
	y = posy;
	w = width;
	h = height;
}
void cBicho::SetPosition(int posx, int posy)
{
	x = posx;
	y = posy;
}
void cBicho::GetPosition(int *posx, int *posy)
{
	*posx = x;
	*posy = y;
}
void cBicho::SetTile(int tx, int ty)
{
	x = tx * TILE_SIZE;
	y = ty * TILE_SIZE;
}
void cBicho::GetTile(int *tx, int *ty)
{
	*tx = x / TILE_SIZE;
	*ty = y / TILE_SIZE;
}
void cBicho::SetWidthHeight(int width, int height)
{
	w = width;
	h = height;
}
void cBicho::GetWidthHeight(int *width, int *height)
{
	*width = w;
	*height = h;
}
void cBicho::SetStepLength(int sl)
{
	step_length = sl;
}
int cBicho::GetStepLength()
{
	return step_length;
}
bool cBicho::Collides(cRect *rc)
{
	//return ((x>rc->left) && (x+w<rc->right) && (y>rc->bottom) && (y+h<rc->top));
	cRect chb = GetCurrentHitbox();
	/*int d1x = rc->left - chb.right;
	int d1y = rc->bottom - chb.top;
	int d2x = chb.left - rc->right;
	int d2y = chb.bottom - rc->top;

	if (d1x > 0 || d1y > 0) return false;
	if(d2x > 0 || d2y > 0) return false;*/

	bool hit = !(
		chb.right < rc->left ||
		rc->right < chb.left ||
		chb.top < rc->bottom ||
		rc->top < chb.bottom
		);
	return hit;
}

bool cBicho::CollidesMapLimits(worldMatrix *map) {
	
	cRect hb = GetCurrentHitbox();
	bool out = false;

	switch (state) {
	case (STATE_LOOKLEFT) :
	case (STATE_WALKLEFT) :
		if (hb.left < 0) out = true;
		break;
	case (STATE_LOOKRIGHT) :
	case (STATE_WALKRIGHT) :
		if (hb.right > 1600) out = true;
		break;
	case (STATE_LOOKDOWN) :
	case (STATE_WALKDOWN) :
		if (hb.bottom < 0) out = true;
		break;
	case (STATE_LOOKUP) :
	case(STATE_WALKUP) :
		if (hb.top > 1120) out = true;
	}

	return out;

}

bool cBicho::CollidesMapWall(worldMatrix *map, bool right)
{
	int tile_x, tile_y;
	int j;
	int width_tiles, height_tiles;

	tile_x = x / TILE_SIZE;
	tile_y = y / TILE_SIZE;
	width_tiles = w / TILE_SIZE;
	height_tiles = h / TILE_SIZE;

	if (right)	tile_x += width_tiles;

	//for (j = 0; j<height_tiles; j++)
	//{
	//	if (map[tile_x + ((tile_y + j)*SCENE_WIDTH)] != 0)	return true;
	//}

	return false;
}

bool cBicho::CollidesMapFloor(worldMatrix *map)
{
	int tile_x, tile_y;
	int width_tiles;
	bool on_base;
	int i;

	tile_x = x / TILE_SIZE;
	tile_y = y / TILE_SIZE;

	width_tiles = w / TILE_SIZE;
	if ((x % TILE_SIZE) != 0) width_tiles++;

	on_base = false;
	i = 0;
	while ((i<width_tiles) && !on_base)
	{
		if ((y % TILE_SIZE) == 0)
		{
			//if (map[(tile_x + i) + ((tile_y - 1) * SCENE_WIDTH)] != 0)
				//on_base = true;
		}
		else
		{
		//	if (map[(tile_x + i) + (tile_y * SCENE_WIDTH)] != 0)
		//	{
		//		y = (tile_y + 1) * TILE_SIZE;
		//		on_base = true;
		//	}
		}
		i++;
	}
	return on_base;
}

void cBicho::GetArea(cRect *rc)
{
	rc->left = x;
	rc->right = x + w;
	rc->bottom = y;
	rc->top = y + h;
}

void cBicho::DrawRect(int tex_id, float xo, float yo, float xf, float yf)
{
	int screen_x, screen_y;

	screen_x = x + SCENE_Xo;
	screen_y = y + SCENE_Yo + (BLOCK_SIZE - TILE_SIZE);

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, tex_id);
	glBegin(GL_QUADS);
	glTexCoord2f(xo, yo);	glVertex2i(screen_x, screen_y);
	glTexCoord2f(xf, yo);	glVertex2i(screen_x + w, screen_y);
	glTexCoord2f(xf, yf);	glVertex2i(screen_x + w, screen_y + h);
	glTexCoord2f(xo, yf);	glVertex2i(screen_x, screen_y + h);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void cBicho::MoveUp(worldMatrix *map) {
	int yaux;

	//Whats next tile?
	/*if ((y % TILE_SIZE) == 0)
	{*/
		yaux = y;
		y += step_length;

		if (CollidesMapLimits(map))
		{
			y = yaux;
			state = STATE_LOOKUP;
		}
	//}
	////Advance, no problem
	//else
	//{
	//	y += step_length;
		else if (state != STATE_WALKUP)
		{
			state = STATE_WALKUP;
			seq = 0;
			delay = 0;
		}
	//}
}

void cBicho::MoveDown(worldMatrix *map) {
	int yaux;

	//Whats next tile?
	/*if ((y % TILE_SIZE) == 0)
	{*/
		yaux = y;
		y -= step_length;

		if (CollidesMapLimits(map))
		{
			y = yaux;
			state = STATE_LOOKDOWN;
		}
	//}
	////Advance, no problem
	//else
	//{
		//y -= step_length;
		else if (state != STATE_WALKDOWN)
		{
			state = STATE_WALKDOWN;
			seq = 0;
			delay = 0;
		}
	//}
}

void cBicho::MoveLeft(worldMatrix *map)
{
	int xaux;

	//Whats next tile?
	///*if ((x % TILE_SIZE) == 0)
	//{*/
		xaux = x;
		x -= step_length;

		if (CollidesMapLimits(map))
		{
			x = xaux;
			state = STATE_LOOKLEFT;
		}

	/*}
	//Advance, no problem
	else
	{
		x -= step_length;*/
		else if (state != STATE_WALKLEFT)
		{
			state = STATE_WALKLEFT;
			seq = 0;
			delay = 0;
		}
	//}
}
void cBicho::MoveRight(worldMatrix *map)
{
	int xaux;

	//Whats next tile?
	/*if ((x % TILE_SIZE) == 0)
	{*/
		xaux = x;
		x += step_length;

		if (CollidesMapLimits(map))
		{
			x = xaux;
			state = STATE_LOOKRIGHT;
		}
	//}
	////Advance, no problem
	//else
	//{
		//x += step_length;

		else if (state != STATE_WALKRIGHT)
		{
			state = STATE_WALKRIGHT;
			seq = 0;
			delay = 0;
		}
	//}
}

void cBicho::Stop()
{
	switch (state)
	{
	case STATE_WALKLEFT:	state = STATE_LOOKLEFT;		break;
	case STATE_WALKRIGHT:	state = STATE_LOOKRIGHT;	break;
	case STATE_WALKUP:		state = STATE_LOOKUP;		break;
	case STATE_WALKDOWN:	state = STATE_LOOKDOWN;		break;
	}
}

void cBicho::Jump(worldMatrix *map)
{
	/*if(!jumping)
	{
	if(CollidesMapFloor(map))
	{
	jumping = true;
	jump_alfa = 0;
	jump_y = y;
	}
	}*/
}

void cBicho::Logic(worldMatrix *map)
{
	/*float alfa;

	if(jumping)
	{
	jump_alfa += JUMP_STEP;

	if(jump_alfa == 180)
	{
	jumping = false;
	y = jump_y;
	}
	else
	{
	alfa = ((float)jump_alfa) * 0.017453f;
	y = jump_y + (int)( ((float)JUMP_HEIGHT) * sin(alfa) );

	if(jump_alfa > 90)
	{
	//Over floor?
	jumping = !CollidesMapFloor(map);
	}
	}
	}
	else
	{
	//Over floor?
	if(!CollidesMapFloor(map))
	y -= (2*step_length);
	}*/
}
void cBicho::NextFrame(int max)
{
	delay++;
	if (delay == frame_delay)
	{
		seq++;
		seq %= max;
		delay = 0;
	}
}
int cBicho::GetFrame()
{
	return seq;
}
void cBicho::SetFrameDelay(int fd)
{
	frame_delay = fd;
}
cRect cBicho::GetHitbox()
{
	return Hitbox;
}
void cBicho::SetHitbox(int left, int right, int bottom, int top)
{
	Hitbox.left = left;
	Hitbox.right = right;
	Hitbox.bottom = bottom;
	Hitbox.top = top;
}
void cBicho::SetFourHitboxes(int left, int right, int bottom, int top, int position)
{
	cRect hb;
	hb.left = left;
	hb.right = right;
	hb.bottom = bottom;
	hb.top = top;
	vHitbox[position] = hb;
}
cRect cBicho::GetHitboxByPosition(int position)
{
	cRect chb;
	chb.left = x + vHitbox[position].left;
	chb.right = x + vHitbox[position].right;
	chb.bottom = y + vHitbox[position].bottom;
	chb.top = y + vHitbox[position].top;
	return chb;
}
cRect cBicho::GetCurrentHitbox()
{
	cRect chb;
	chb.left = x + Hitbox.left;
	chb.right = x + Hitbox.right;
	chb.bottom = y + Hitbox.bottom;
	chb.top = y + Hitbox.top;
	return chb;
}
int cBicho::GetState()
{
	return state;
}
void cBicho::SetState(int s)
{
	state = s;
}

float cBicho::GetLives()
{
	return lives;
}

void cBicho::SetLives(float l)
{
	lives = l;
}

bool cBicho::GetAlive()
{
	return (lives > 0);
}

void cBicho::SetAlive(bool a)
{
	alive = a;
}

bool cBicho::GetHit()
{
	return hit;
}

void cBicho::SetHit(bool h)
{
	hit = h;
}

void cBicho::Hurt()
{
	hurt = true;
	--lives;
	if (lives == 0) alive = false;
	else {
		immune = true;
		SetImmuneTime(100);
	}
}

bool cBicho::GetHurt()
{
	return hurt;
}

void cBicho::SetHurt(bool h)
{
	hurt = h;
}

bool cBicho::GetImmune()
{
	return immune;
}

void cBicho::SetImmune(bool i)
{
	immune = i;
}

int cBicho::GetImmuneTime()
{
	return immune_time;
}

void cBicho::SetImmuneTime(int it)
{
	immune_time = it;
}

bool cBicho::GetWeaponThrown()
{
	return weaponThrown;
}

void cBicho::SetWeaponThrown(bool wt)
{
	weaponThrown = wt;
}

bool cBicho::GetWeaponHit()
{
	return weaponHit;
}

void cBicho::SetWeaponHit(bool wh)
{
	weaponHit = wh;
}
