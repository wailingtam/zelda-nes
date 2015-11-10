#pragma once

#include "cTexture.h"
#include "cScene.h"
#include "Globals.h"

#define FRAME_DELAY		8
#define JUMP_HEIGHT		48
#define JUMP_STEP		4

#define STATE_LOOKLEFT		0
#define STATE_LOOKRIGHT		1
#define STATE_LOOKUP		2
#define STATE_LOOKDOWN		3
#define STATE_WALKLEFT		4
#define STATE_WALKRIGHT		5
#define STATE_WALKUP		6
#define	STATE_WALKDOWN		7
#define STATE_IDLE			8

#define LINK				0
#define OCTOROK				1
#define WIZZROBE			2
#define AQUAMENTUS			3

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
	void SetStepLength(int sl);
	int	 GetStepLength();

	bool Collides(cRect *rc);
	bool CollidesMapLimits(worldMatrix * map);
	bool isBlocking(worldMatrix * map);
	void GetArea(cRect *rc);
	void DrawRect(int tex_id, float xo, float yo, float xf, float yf);

	void MoveRight(worldMatrix *map);
	void MoveLeft(worldMatrix *map);
	void MoveUp(worldMatrix *map);
	void MoveDown(worldMatrix *map);
	void Jump(worldMatrix *map);
	void Stop();
	void Logic(worldMatrix *map);

	int  GetState();
	void SetState(int s);
	float GetLives();
	void SetLives(float l);
	bool GetAlive();
	void SetAlive(bool a);
	bool GetHit();
	void SetHit(bool h);
	void Hurt();
	bool GetHurt();
	void SetHurt(bool h);
	bool GetImmune();
	void SetImmune(bool i);
	int GetImmuneTime();
	void SetImmuneTime(int it);
	bool GetWeaponThrown();
	void SetWeaponThrown(bool wt);
	bool GetWeaponHit();
	void SetWeaponHit(bool wh);

	void NextFrame(int max);
	int  GetFrame();
	void SetFrameDelay(int fd);

	cRect GetHitbox();
	void SetHitbox(int left, int right, int bottom, int top);
	void SetFourHitboxes(int left, int right, int bottom, int top, int position);
	cRect GetHitboxByPosition(int position);
	cRect GetCurrentHitbox();

	cRect Hitbox;
	cRect vHitbox[4];

private:
	int x, y;
	int w, h;
	int state;
	float lives;
	bool alive;
	bool weaponThrown;
	bool weaponHit;
	bool hit;
	bool hurt;
	bool immune;
	int immune_time;
	int step_length;

	int seq, delay;
	int frame_delay;
};
