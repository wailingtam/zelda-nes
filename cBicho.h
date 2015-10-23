#pragma once

#include "cTexture.h"
#include "Globals.h"

#define FRAME_DELAY		8
#define STEP_LENGTH		2
#define JUMP_HEIGHT		96
#define JUMP_STEP		4

#define STATE_LOOKLEFT		0
#define STATE_LOOKRIGHT		1
#define STATE_LOOKUP		2
#define STATE_LOOKDOWN		3
#define STATE_WALKLEFT		4
#define STATE_WALKRIGHT		5
#define STATE_WALKUP		6
#define	STATE_WALKDOWN		7

class cRect
{
public:
	int left, top,
		right, bottom;
};

class cBicho
{
public:
	cBicho(void);
	cBicho(int x, int y, int w, int h);
	~cBicho(void);

	void SetPosition(int x, int y);
	void GetPosition(int *x, int *y);
	void SetTile(int tx, int ty);
	void GetTile(int *tx, int *ty);
	void SetWidthHeight(int w, int h);
	void GetWidthHeight(int *w, int *h);

	bool Collides(cRect *rc);
	bool CollidesMapWall(int *map, bool right);
	bool CollidesMapFloor(int *map);
	void GetArea(cRect *rc);
	void DrawRect(int tex_id, float xo, float yo, float xf, float yf);

	void MoveRight(int *map);
	void MoveLeft(int *map);
	void MoveUp(int* map);
	void MoveDown(int* map);
	void Attack(int* map);
	void Jump(int *map);
	void Stop();
	void Logic(int *map);

	int  GetState();
	void SetState(int s);
	bool GetAttackState();
	void SetAttackState(bool as);
	int	 GetLives();
	void SetLives(int l);

	void NextFrame(int max);
	int  GetFrame();

private:
	int x, y;
	int w, h;
	int state;
	int attackstate;
	int lives;

	bool jumping;
	bool attacking;
	int jump_alfa;
	int jump_y;

	int seq, delay;
};
