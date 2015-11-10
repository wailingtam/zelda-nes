#include "cGame.h"
#include "Globals.h"


cGame::cGame(void)
{
}

cGame::~cGame(void)
{
}

void cGame::setView(int w, int h) {
	glViewport(0, 0, w * ZOOM_FACTOR, h * ZOOM_FACTOR);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, 0, 1);
	glMatrixMode(GL_MODELVIEW);

	glAlphaFunc(GL_GREATER, 0.05f);
	glEnable(GL_ALPHA_TEST);
	this->w = w;
	this->h = h;
}

bool cGame::Init()
{
	bool res=true;

	cGame::setView(GAME_WIDTH, GAME_HEIGHT);

	//Scene initialization
	Data.loadOverworldTextures();
	Scene.loadOverworld();
	Scene.loadInnerworld();

	//Player initialization
	Data.LoadImage(IMG_PLAYER, "resources/tileset/link.png", GL_RGBA);
	Data.LoadImage(IMG_ENEMIES, "resources/tileset/enemies.png", GL_RGBA);

	Data.loadImage(IMG_HEART, "resources/tileset/heart.png", GL_RGBA);
	Data.loadImage(IMG_MINIMAP, "resources/minimap.png", GL_RGBA);

	srand(time(NULL));

	Sound.Init();
	soundsLoading();
	Sound.Play(OVERWORLD_BGM, MUSIC_CHANNEL1);
	/*Sound.SetVolume(MUSIC_CG, 0.1f);
	Sound.SetVolume(EFFECTS_CG, 0.1f);*/

	Player.Init();

	spawn(0);

	return res;
}

bool cGame::Loop()
{
	bool res=true;

	res = Process();
	if(res) Render();

	return res;
}

void cGame::Finalize()
{
}

//Input
void cGame::ReadKeyboard(unsigned char key, int x, int y, bool press)
{
	keys[key] = press;
}

void cGame::ReadMouse(int button, int state, int x, int y)
{
}

//Process
bool cGame::Process()
{
	Sound.UpdateSound();
	int oldx, oldy, newx, newy;
	Player.GetPosition(&oldx, &oldy);
	bool res = true;
	int level = this->isOverworld ? OVERWORLD_LEVEL : INNERWORLD_LEVEL;
	int otherLevel = this->isOverworld ? INNERWORLD_LEVEL : OVERWORLD_LEVEL;
	bool oldLevel = this->isOverworld;
	bool levelChanged = false;
	//Process Input
	if (keys[27])	res = false;
	
	if (keys[GLUT_KEY_UP])			Player.MoveUp(Scene.GetMap(level), Scene.GetMap(otherLevel), &this->isOverworld);
	else if (keys[GLUT_KEY_DOWN])	Player.MoveDown(Scene.GetMap(level), Scene.GetMap(otherLevel), &this->isOverworld);
	else if (keys[GLUT_KEY_LEFT])	Player.MoveLeft(Scene.GetMap(level), Scene.GetMap(otherLevel), &this->isOverworld);
	else if (keys[GLUT_KEY_RIGHT])	Player.MoveRight(Scene.GetMap(level), Scene.GetMap(otherLevel), &this->isOverworld);
	else if (keys[GLUT_KEY_F1]) this->isOverworld = !this->isOverworld;
	else if (keys[97]) {				//A = 97, S = 115
		Player.Attack(Scene.GetMap(level));		
		Sound.Play(SWORD, EFFECTS_CHANNEL1);
		if (Player.GetThrowing())
			Sound.Play(SWORD_SHOOT, EFFECTS_CHANNEL1);
	}
	else Player.Stop();

	if (oldLevel != this->isOverworld) {  //We change 'level' value accordingly
		levelChanged = true;
		int aux = level;
		level = otherLevel;
		otherLevel = level;
	}

	Player.GetPosition(&newx, &newy);
	//int zone = this->getNewSpanZone(oldx, oldy, newx, newy, levelChanged);
	int zone = -1;
	if (zone != -1) this->spawn(zone);

	//Game Logic
	gameLogic(level);

	Player.countTime();

	return res;
}

//Output
void cGame::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();
		moveCamera();
		Scene.setDrawing(this->isOverworld ? OVERWORLD_LEVEL : INNERWORLD_LEVEL);
		Scene.Draw(Data.getOverworldIds());
		
		if (Player.GetAlive()) Player.Draw(Data.GetID(IMG_PLAYER));
		for (int i = 0; i < vOctorok.size(); i++) {
			if (vOctorok[i]->GetAlive()) vOctorok[i]->Draw(Data.GetID(IMG_ENEMIES));
		}
		for (int i = 0; i < vTektike.size(); i++) {
			if (vTektike[i]->GetAlive()) vTektike[i]->Draw(Data.GetID(IMG_ENEMIES));
		}
		for (int i = 0; i < vWizzrobe.size(); i++) {
			if (vWizzrobe[i]->GetAlive()) vWizzrobe[i]->Draw(Data.GetID(IMG_ENEMIES));
		}
		if (Aquamentus.GetAlive()) Aquamentus.Draw(Data.GetID(IMG_ENEMIES));

		Scene.DrawAboveBichos(Data.getOverworldIds());

	glLoadIdentity();
	//glOrtho(0, this->w, 0, this->h, 0, 1);
	//glMatrixMode(GL_MODELVIEW);
		Interface.drawLive(Data.GetID(IMG_HEART), Player.GetLives(), this->h);
		if (this->isOverworld) {
			int x, y;
			Player.GetPosition(&x, &y);
			worldMatrix *map = Scene.GetMap(OVERWORLD_LEVEL);
			Interface.drawMinimap(Data.GetID(IMG_MINIMAP), (float)x / (float)((*map)[0].size() * BLOCK_SIZE), (float)y / (float)(map->size() * BLOCK_SIZE));
		}
	glutSwapBuffers();
}


void cGame::moveCamera() {
	//glTranslatef(18 , 5 , 0);
	int playerx, playery;
	Player.GetPosition(&playerx, &playery);
	glTranslatef(-playerx, -playery, 0);	//Move the camera with the player
	glTranslatef(w / (2.0 * ZOOM_FACTOR) - 24, ((h / ZOOM_FACTOR) / 2.0) - 24, 0);
}


void cGame::spawn(int zone) {
	respawnOfBichos *res = &this->respawnZones[zone].respawn;
	int level = this->isOverworld ? OVERWORLD_LEVEL : INNERWORLD_LEVEL;
	int i = 0;
	for (position pos : res->octorocks) {
		vOctorok.push_back(new cOctorok());
		vOctorok[i++]->SetTile(pos.x, Scene.GetMap(level)->size() - pos.y);
	}
	i = 0;
	for (position pos : res->tektikes) {
		vTektike.push_back(new cTektike());
		vTektike[i++]->SetTile(pos.x, Scene.GetMap(level)->size() - pos.y);
	}
	i = 0;
	for (position pos : res->wizzrobes) {
		vWizzrobe.push_back(new cWizzrobe());
		vWizzrobe[i++]->SetTile(pos.x, Scene.GetMap(level)->size() - pos.y);
	}
	//Aquamentus.Init(); ??
}

void cGame::soundsLoading() {
	Sound.LoadSound(OVERWORLD_BGM, "resources/audio/Overworld_BGM.ogg", BACKGROUND);
	Sound.LoadSound(SWORD, "resources/audio/LOZ_Sword.wav", EFFECT);
	Sound.LoadSound(SWORD_SHOOT, "resources/audio/LOZ_Sword_Shoot.wav", EFFECT);
	Sound.LoadSound(HURT, "resources/audio/LOZ_Hurt.wav", EFFECT);
	Sound.LoadSound(HIT, "resources/audio/LOZ_Hit.wav", EFFECT);
	Sound.LoadSound(KILL, "resources/audio/LOZ_Kill.wav", EFFECT);
	Sound.LoadSound(LOW_HEALTH, "resources/audio/LOZ_LowHealth.wav", BACKGROUND);
	Sound.LoadSound(DIE, "resources/audio/LOZ_Dead.ogg", EFFECT);
}

void cGame::gameLogic(int level) {
	cRect pchb = Player.GetCurrentHitbox();
	if (Player.GetAlive()) Player.Logic(Scene.GetMap(level), &pchb);
	if (Player.GetLives() > 0 && Player.GetLives() <= 1.0 && !Player.GetLow()) {
		Sound.Play(LOW_HEALTH, EFFECTS_CHANNEL2);
		Player.SetLow(true);
	}
	else if (Player.GetLives() > 1.0 && Player.GetLow() || Player.GetLives() == 0) {
		Sound.PauseChannel(EFFECTS_CHANNEL2);
		Player.SetLow(false);
	}
	if (Player.GetLives() == 0) {
		Sound.Play(DIE, EFFECTS_CHANNEL1);
		Player.SetLives(-1.0);
	}
	pchb = Player.GetCurrentHitbox();
	cRect shb = Player.GetSwordHitbox();
	bool directAttack = Player.GetAttacking();
	cRect dshb;
	if (directAttack) dshb = Player.GetHitboxByPosition(Player.GetState());
	bool swordThrown = Player.GetWeaponThrown();

	int px, py;
	Player.GetPosition(&px, &py);

	for (int i = 0; i < vOctorok.size(); i++) {
		if (vOctorok[i]->GetAlive()) {
			vOctorok[i]->Logic(Scene.GetMap(level), &pchb, &shb, &dshb, swordThrown, directAttack);
			if (vOctorok[i]->GetWeaponHit() || vOctorok[i]->GetHit()) {
				if (!Player.GetImmune()) {
					Player.Hurt();
					if (vOctorok[i]->GetWeaponHit()) vOctorok[i]->SetWeaponThrown(false);
					Sound.Play(HURT, EFFECTS_CHANNEL1);
				}
				vOctorok[i]->SetWeaponHit(false);
				vOctorok[i]->SetHit(false);
			}
			if (vOctorok[i]->GetHurt()) {
				Sound.Play(KILL, EFFECTS_CHANNEL1);
				Player.SetWeaponThrown(false);
				vOctorok[i]->SetHurt(false);
			}
		}
	}

	for (int i = 0; i < vTektike.size(); i++) {
		if (vTektike[i]->GetAlive()) {
			vTektike[i]->Logic(Scene.GetMap(level), &pchb, &shb, &dshb, swordThrown, directAttack);
			if (vTektike[i]->GetHit()) {
				if (!Player.GetImmune()) {
					Player.Hurt();
					Sound.Play(HURT, EFFECTS_CHANNEL1);
				}
				vTektike[i]->SetHit(false);
			}
			if (vTektike[i]->GetHurt()) {
				Sound.Play(KILL, EFFECTS_CHANNEL1);
				Player.SetWeaponThrown(false);
				vTektike[i]->SetHurt(false);
			}
		}
	}

	for (int i = 0; i < vWizzrobe.size(); i++) {
		if (vWizzrobe[i]->GetAlive()) {
			vWizzrobe[i]->Logic(Scene.GetMap(level), px, py, &pchb, &shb, &dshb, swordThrown, directAttack);
			if (vWizzrobe[i]->GetWeaponHit() || vWizzrobe[i]->GetHit()) {
				if (!Player.GetImmune()) {
					Player.Hurt();
					Sound.Play(HURT, EFFECTS_CHANNEL1);
					if (vWizzrobe[i]->GetWeaponHit()) vWizzrobe[i]->SetWeaponThrown(false);
				}
				vWizzrobe[i]->SetWeaponHit(false);
				vWizzrobe[i]->SetHit(false);
			}
			if (vWizzrobe[i]->GetHurt()) {
				Sound.Play(KILL, EFFECTS_CHANNEL1);
				Player.SetWeaponThrown(false);
				vWizzrobe[i]->SetHurt(false);
			}
		}
	}

	if (Aquamentus.GetAlive()) {
		Aquamentus.Logic(Scene.GetMap(level), &pchb, &shb, &dshb, swordThrown, directAttack);
		if (Aquamentus.GetWeaponHit() || Aquamentus.GetHit()) {
			if (!Player.GetImmune()) {
				Player.Hurt();
				if (Aquamentus.GetWeaponHit()) Aquamentus.SetWeaponHit(false);
				Sound.Play(HURT, EFFECTS_CHANNEL1);
			}
			Aquamentus.SetHit(false);
		}
		if (Aquamentus.GetHurt()) {
			if (!Aquamentus.GetAlive()) Sound.Play(KILL, EFFECTS_CHANNEL1);
			else Sound.Play(HIT, EFFECTS_CHANNEL1);
			Player.SetWeaponThrown(false);
			Aquamentus.SetHurt(false);
		}
	}
}