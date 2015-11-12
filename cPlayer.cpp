#include "cPlayer.h"

cPlayer::cPlayer() {}
cPlayer::~cPlayer() {}


void cPlayer::Init()
{
	SetFrameDelay(8);
	SetWidthHeight(46, 46);
	//SetTile(6, 3);
	SetTile(18, 5);
	//SetTile(0, 0);
	SetState(STATE_LOOKDOWN);
	SetStepLength(2);
	SetLives(3.0);
	attacking = false;
	SetImmune(true);
	SetImmuneTime(150);
	Sword.SetWidthHeight(46, 46);
	Sword.SetThrown(false);
	Sword.SetSpeed(4);
	SetHitbox(16, 28, 16, 30);
	SetFourHitboxes(4, 15, 21, 24, 0);
	SetFourHitboxes(31, 42, 21, 24, 1);
	SetFourHitboxes(21, 24, 31, 42, 2);
	SetFourHitboxes(22, 25, 5, 16, 3);
	Sword.SetFourHitboxes(4, 20, 21, 24, 0);
	Sword.SetFourHitboxes(26, 42, 21, 24, 1);
	Sword.SetFourHitboxes(21, 24, 26, 42, 2);
	Sword.SetFourHitboxes(22, 25, 5, 21, 3);
	low = false;
	weaponIsSword = false;

	Boomerang.SetFrameDelay(8);
	Boomerang.SetWidthHeight(46, 46);
	Boomerang.SetThrown(false);
	Boomerang.SetSpeed(4);
	Boomerang.SetComingBack(false);
	Boomerang.SetHitbox(17, 29, 17, 28);
}

void cPlayer::Logic(worldMatrix * map, cRect *playerHitbox)
{
	/*if (!GetWeaponThrown() && Sword.GetThrown() || Boomerang.GetThrown()) {
		Sword.SetThrown(false);
		Boomerang.SetThrown(false);
	}*/

	if (GetWeaponThrown()) {
		int px, py;
		GetPosition(&px, &py);
		if (weaponIsSword) {
			if (!Sword.GetThrown()) {
				Sword.SetPosition(px, py);
				Sword.SetState(GetState());
				Sword.SetThrown(true);
				Sword.ResetDistance();
			}
			Sword.Logic(map, playerHitbox);
			if (!Sword.GetThrown()) SetWeaponThrown(false);
		}
		else {
			if (!Boomerang.GetThrown()) {
				Boomerang.SetPosition(px, py);
				Boomerang.SetState(GetState());
				Boomerang.SetThrown(true);
				Boomerang.ResetDistance();
			}
			Boomerang.SetObjective(px, py);
			Boomerang.Logic(map, playerHitbox);
			if (!Boomerang.GetThrown()) SetWeaponThrown(false);
		}
	}
	else {
		Sword.SetThrown(false);
		Boomerang.SetThrown(false);
	}
}

void cPlayer::Draw(int tex_id)
{
	float xo, yo, xf, yf;

	switch(GetState())
	{
		case STATE_LOOKLEFT:	if (attacking || throwing && weaponIsSword) {
									xo = 46.0f / 256.0f;		yo = 184.0f / 256.0f;
									attacking = false;
									throwing = false;
								}
								else if (throwing && !weaponIsSword) {
									xo = 46.0f / 256.0f;		yo = 138.0f / 256.0f;
									throwing = false;
								}
								else { xo = 46.0f / 256.0f;	yo = 46.0f / 256.0f; }
								break;
		case STATE_LOOKRIGHT:	if (attacking || throwing && weaponIsSword) {
									xo = 138.0f / 256.0f;		yo = 184.0f / 256.0f;
									attacking = false;
									throwing = false;
								}
								else if (throwing && !weaponIsSword) {
									xo = 138.0f / 256.0f;		yo = 138.0f / 256.0f;
									throwing = false;
								}
								else { xo = 138.0f / 256.0f;	yo = 46.0f / 256.0f; }
								break;

		case STATE_LOOKUP:		if (attacking || throwing && weaponIsSword) {
									xo = 92.0f / 256.0f;		yo = 184.0f / 256.0f;
									attacking = false;
									throwing = false;
								}
								else if (throwing && !weaponIsSword) {
									xo = 92.0f / 256.0f;		yo = 138.0f / 256.0f;
									throwing = false;
								}
								else { xo = 92.0f / 256.0f;			yo = 46.0f / 256.0f; }
								break;

		case STATE_LOOKDOWN:	if (attacking || throwing && weaponIsSword) {
									xo = 0.0f;					yo = 184.0f / 256.0f;
									attacking = false;
									throwing = false;
								}
								else if (throwing && !weaponIsSword) {
									xo = 0.0f;					yo = 138.0f / 256.0f;
									throwing = false;
								}
								else { xo = 0.0f;				yo = 46.0f / 256.0f; }
								break;

		case STATE_WALKLEFT:	if (attacking || throwing && weaponIsSword) {
									xo = 46.0f / 256.0f;		yo = 184.0f / 256.0f;
									attacking = false;
									throwing = false;
								}
								else if (throwing && !weaponIsSword) {
									xo = 46.0f / 256.0f;		yo = 138.0f / 256.0f;
									throwing = false;
								}
								else {
									xo = 46.0f / 256.0f;	yo = 46.0f / 256.0f + (GetFrame()*46.0f / 256.0f);
									NextFrame(2);
								}
								break;

		case STATE_WALKRIGHT:	if (attacking || throwing && weaponIsSword) {
									xo = 138.0f / 256.0f;		yo = 184.0f / 256.0f;
									attacking = false;
									throwing = false;
								}
								else if (throwing && !weaponIsSword) {
									xo = 138.0f / 256.0f;		yo = 138.0f / 256.0f;
									throwing = false;
								}
								else {
									xo = 138.0f / 256.0f;	yo = 46.0f / 256.0f + (GetFrame()*46.0f / 256.0f);
									NextFrame(2);
								}
								break;

		case STATE_WALKUP:		if (attacking || throwing && weaponIsSword) {
									xo = 92.0f / 256.0f;		yo = 184.0f / 256.0f;
									attacking = false;
									throwing = false;
								}
								else if (throwing && !weaponIsSword) {
									xo = 92.0f / 256.0f;		yo = 138.0f / 256.0f;
									throwing = false;
								}
								else {
									xo = 92.0f / 256.0f;	yo = 46.0f / 256.0f + (GetFrame()*46.0f / 256.0f);
									NextFrame(2);
								}
								break;

		case STATE_WALKDOWN:	if (attacking || throwing && weaponIsSword) {
									xo = 0.0f;					yo = 184.0f / 256.0f;
									attacking = false;
									throwing = false;
								}
								else if (throwing && !weaponIsSword) {
									xo = 0.0f;					yo = 138.0f / 256.0f;
									throwing = false;
								}
								else {
									xo = 0.0f;	yo = 46.0f / 256.0f + (GetFrame()*46.0f / 256.0f);
									NextFrame(2);
								}
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
	else if (Boomerang.GetThrown()) Boomerang.Draw(tex_id);
}

void cPlayer::Attack() {

	if (weaponIsSword) {
		if (GetLives() == 3 && !GetWeaponThrown()) {
			SetWeaponThrown(true);
			throwing = true;
		}
		else attacking = true;
	}
	else {
		if (!GetWeaponThrown()) {
			SetWeaponThrown(true);
			throwing = true;
		}
	}
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

bool cPlayer::GetAttacking()
{
	return attacking;
}

bool cPlayer::GetThrowing()
{
	return throwing;
}

bool cPlayer::GetLow()
{
	return low;
}

void cPlayer::SetLow(bool l)
{
	low = l;
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

cRect cPlayer::GetBoomerangHitbox()
{
	cRect bhb = Boomerang.GetCurrentHitbox();
	return bhb;
}

void cPlayer::SetBoomerangComingBack()
{
	Boomerang.SetComingBack(true);
}

bool cPlayer::usingSword()
{
	return weaponIsSword;
}

void cPlayer::SetUsingSword(bool s)
{
	if (!GetWeaponThrown()) weaponIsSword = s;
}

void cPlayer::countTime() {
	unsigned int time = glutGet(GLUT_ELAPSED_TIME);
	if (time - this->initialTime >= SPELL_RECHARGE_TIME)
		this->_canUseMagic = true;
	if (time - initialTime >= SPELL_DURATION)
		spellActive = false;
}

void cPlayer::castSpell() {
	if (this->_canUseMagic) {
		this->initialTime = glutGet(GLUT_ELAPSED_TIME);
		spellActive = true;
		_canUseMagic = false;
	}
}

bool cPlayer::canUseMagic() {
	return this->_canUseMagic;
}

bool cPlayer::GetSpellActive()
{
	return spellActive;
}

bool cPlayer::teleport(worldMatrix *map, worldMatrix *otherMap, TeleportDirection tpdir){
	cRect hb = GetCurrentHitbox();
	int y1 = map->size() - 1 - hb.bottom / TILE_SIZE;	//Vectors are size - 1!
	square *sleft = &(*map)[y1][hb.left / TILE_SIZE];
	square *sright = &(*map)[y1][hb.right / TILE_SIZE];
	
	int extraY = tpdir == tpUp ? TILE_SIZE : (tpdir == tpDown? -TILE_SIZE : 0) - TILE_SIZE; //The first TILE_SIZE of the ? is to add an extra tile. The - TILE_SIZE of the end is for the box of the zelda.
	int extraX = tpdir == tpLeft ? -TILE_SIZE : (tpdir == tpRight? TILE_SIZE : 0) - TILE_SIZE;
	if (sleft->changeLevel) this->SetPosition(sleft->newpos.x * TILE_SIZE + extraX, (otherMap->size() - 1 - sleft->newpos.y) * TILE_SIZE + extraY); //Vectors are -1
	else if(sright->changeLevel) this->SetPosition(sright->newpos.x * TILE_SIZE + extraX, (otherMap->size() - 1 - sright->newpos.y) * TILE_SIZE + extraY);
	return sleft->changeLevel || sright->changeLevel;
}

void cPlayer::MoveRight(worldMatrix *map, worldMatrix *otherMap, bool *isOverworld) {
	cBicho::MoveRight(map);
	if (teleport(map, otherMap, tpRight)) (*isOverworld) = !(*isOverworld);
}
void cPlayer::MoveLeft(worldMatrix *map, worldMatrix *otherMap, bool *isOverworld) {
	cBicho::MoveLeft(map);
	if (teleport(map, otherMap, tpLeft)) (*isOverworld) = !(*isOverworld);
}
void cPlayer::MoveUp(worldMatrix *map, worldMatrix *otherMap, bool *isOverworld) {
	cBicho::MoveUp(map);
	if (teleport(map, otherMap, tpUp)) (*isOverworld) = !(*isOverworld);
}
void cPlayer::MoveDown(worldMatrix *map, worldMatrix *otherMap, bool *isOverworld) {
	cBicho::MoveDown(map);
	if (teleport(map, otherMap, tpDown)) (*isOverworld) = !(*isOverworld);
}