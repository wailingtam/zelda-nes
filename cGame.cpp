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
	Data.loadImage(IMG_BOW, "resources/tileset/boomerang.png", GL_RGBA);
	Data.loadImage(IMG_SWORD, "resources/tileset/sword.png", GL_RGBA);
	Data.LoadImage(IMG_MAGIC, "resources/tileset/magic.png", GL_RGBA);

	srand(time(NULL));

	Sound.Init();
	soundsLoading();
	Sound.Play(OVERWORLD_BGM, MUSIC_CHANNEL1);
	Sound.SetVolume(MUSIC_CG, 0.5f);
	Sound.SetVolume(EFFECTS_CG, 0.5f);

	Player.Init();

	currentZone = 0;
	spawn(currentZone);

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
	else if (keys[97]) {				//key = a
		Player.Attack();		
		if (Player.usingSword()) {
			Sound.Play(SWORD, EFFECTS_CHANNEL1);
			if (Player.GetThrowing())
				Sound.Play(SWORD_SHOOT, EFFECTS_CHANNEL1);
		}
		else {
			Sound.Play(BOOMERANG, EFFECTS_CHANNEL3);
			Sound.SetChannel3Paused(false);
		}
	}
	else Player.Stop();

	if (keys[119] && (glutGet(GLUT_ELAPSED_TIME) - switchTime) > 500) {
		Player.SetUsingSword(!Player.usingSword());		//key = w
		this->switchTime = glutGet(GLUT_ELAPSED_TIME);
	}
	else if (keys[113] && Player.canUseMagic()) {					//key = q
		Player.castSpell();
		Sound.Play(SPELL, EFFECTS_CHANNEL1);
		Sound.PauseChannel(MUSIC_CHANNEL1);
		Sound.SetChannel1Paused(true);
	}

	if (!Player.GetSpellActive() && Sound.GetChannel1Paused()) {
		Sound.Play(OVERWORLD_BGM, MUSIC_CHANNEL1);
		Sound.SetChannel1Paused(false);
	}

	if (oldLevel != this->isOverworld) {  //We change 'level' value accordingly
		levelChanged = true;
		int aux = level;
		level = otherLevel;
		otherLevel = level;
		Sound.PauseChannel(MUSIC_CHANNEL1);
		if (level == INNERWORLD_LEVEL) Sound.Play(UNDERWORLD_BGM, MUSIC_CHANNEL1);
		else Sound.Play(OVERWORLD_BGM, MUSIC_CHANNEL1);
	}
	
	int zone = getNewSpanZone(); //Zone 4 es la única que no está en el overworld
	if (zone != currentZone && zone != -1) {
		spawn(zone);
		currentZone = zone;
	}

	//Game Logic
	gameLogic(level);

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

		for (int i = 0; i < explosions.size(); i++) {
			if (!explosions[i].stop) {
				explosions[i].e1.draw(0, IMG_ENEMIES);
				explosions[i].e2.draw(1, IMG_ENEMIES);
				explosions[i].e3.draw(2, IMG_ENEMIES);
				explosions[i].e4.draw(3, IMG_ENEMIES);
			}
		}

		Scene.DrawAboveBichos(Data.getOverworldIds());

	glLoadIdentity();
	//glOrtho(0, this->w, 0, this->h, 0, 1);
	//glMatrixMode(GL_MODELVIEW);
		Interface.drawLive(Data.GetID(IMG_HEART), Player.GetLives(), this->h);
		Interface.drawWeapon(Player.usingSword()? Data.GetID(IMG_SWORD) : Data.GetID(IMG_BOW), Player.usingSword());
		if(Player.canUseMagic()) Interface.drawMagic(Data.GetID(IMG_MAGIC));
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
	vOctorok.clear();
	vTektike.clear();
	vWizzrobe.clear();
	explosions.clear();
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
	if (zone == 4) {
		Aquamentus.Init();
		Aquamentus.SetTile(res->aquamentus.x, Scene.GetMap(level)->size() - res->aquamentus.y);
	}
}

int cGame::getNewSpanZone()
{
	int tx, tty;
	Player.GetTile(&tx, &tty);
	int level = this->isOverworld ? OVERWORLD_LEVEL : INNERWORLD_LEVEL;
	int ty = Scene.GetMap(level)->size() - 1 - tty;
	bool zoneFound = false;
	int zone = -1;
	int i = 0;
	if (isOverworld) {
		while (!zoneFound && i < respawnZones.size()-1) {
			if (tx >= respawnZones[i].vertexs[0].x && tx <= respawnZones[i].vertexs[1].x
				&& ty >= respawnZones[i].vertexs[2].y && ty <= respawnZones[i].vertexs[0].y) {
				zoneFound = true;
				zone = i;
			}
			++i;
		}
	}
	else zone = 4;

	return zone;
}

void cGame::soundsLoading() {
	Sound.LoadSound(OVERWORLD_BGM, "resources/audio/Overworld_BGM.ogg", BACKGROUND);
	Sound.LoadSound(UNDERWORLD_BGM, "resources/audio/Underworld_BGM.ogg", BACKGROUND);
	Sound.LoadSound(SWORD, "resources/audio/LOZ_Sword.wav", EFFECT);
	Sound.LoadSound(SWORD_SHOOT, "resources/audio/LOZ_Sword_Shoot.wav", EFFECT);
	Sound.LoadSound(HURT, "resources/audio/LOZ_Hurt.wav", EFFECT);
	Sound.LoadSound(HIT, "resources/audio/LOZ_Hit.wav", EFFECT);
	Sound.LoadSound(KILL, "resources/audio/LOZ_Kill.wav", EFFECT);
	Sound.LoadSound(LOW_HEALTH, "resources/audio/LOZ_LowHealth.wav", BACKGROUND);
	Sound.LoadSound(DIE, "resources/audio/LOZ_Dead.ogg", EFFECT);
	Sound.LoadSound(BOOMERANG, "resources/audio/LOZ_Boomerang.wav", BACKGROUND);
	Sound.LoadSound(SPELL, "resources/audio/Spell.mp3", EFFECT);
	Sound.LoadSound(AQUAMENTUS_KILLED, "resources/audio/Aquamentus_killed.wav", EFFECT);
	Sound.LoadSound(SHIELD, "resources/audio/LOZ_Shield.wav", EFFECT);
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
	if (!Player.GetAlive()) {
		if (!gameOver) {
			Sound.PauseChannel(MUSIC_CHANNEL1);
			Sound.Play(DIE, EFFECTS_CHANNEL1);
			this->switchTime = glutGet(GLUT_ELAPSED_TIME);
			gameOver = true;
		}
		if (gameOver && glutGet(GLUT_ELAPSED_TIME) - switchTime > 10000) {
			Sound.Init();
			soundsLoading();
			Sound.Play(OVERWORLD_BGM, MUSIC_CHANNEL1);
			Player.Init();
			currentZone = 0;
			spawn(currentZone);
			gameOver = false;
		}
		
	}
	pchb = Player.GetCurrentHitbox();
	cRect whb;
	if (Player.usingSword()) whb = Player.GetSwordHitbox();
	else whb = Player.GetBoomerangHitbox();
	bool directAttack = Player.GetAttacking();
	cRect dshb;
	if (directAttack) dshb = Player.GetHitboxByPosition(Player.GetState());
	bool playersWeaponThrown = Player.GetWeaponThrown();
	if (!Player.usingSword() && !Player.GetWeaponThrown() && !Sound.GetChannel3Paused()) {
		Sound.PauseChannel(EFFECTS_CHANNEL3);
		Sound.SetChannel3Paused(true);
	}
	int px, py;
	Player.GetPosition(&px, &py);
	bool underSpell = Player.GetSpellActive();

	for (int i = 0; i < vOctorok.size(); i++) {
		if (vOctorok[i]->GetAlive()) {
			vOctorok[i]->Logic(Scene.GetMap(level), &pchb, &whb, &dshb, playersWeaponThrown, directAttack, px, py, underSpell);
			int x, y, s;
			vOctorok[i]->GetRockInfo(&x, &y, &s);
			if (vOctorok[i]->GetWeaponHit() && !shieldProtected(x, y, s) || vOctorok[i]->GetHit()) {
				if (!Player.GetImmune()) {
					Player.Hurt();
					if (vOctorok[i]->GetWeaponHit()) vOctorok[i]->SetWeaponThrown(false);
					Sound.Play(HURT, EFFECTS_CHANNEL1);
				}
				vOctorok[i]->SetWeaponHit(false);
				vOctorok[i]->SetHit(false);
			}
			else if (vOctorok[i]->GetWeaponHit() && shieldProtected(x, y, s)) {
				vOctorok[i]->SetWeaponThrown(false);
				vOctorok[i]->SetWeaponHit(false);
				vOctorok[i]->SetHit(false);
				Sound.Play(SHIELD, EFFECTS_CHANNEL1);
			}
			if (vOctorok[i]->GetHurt()) {
				Sound.Play(KILL, EFFECTS_CHANNEL1);
				int x, y;
				vOctorok[i]->GetPosition(&x, &y);
				newExplosion(x,y);
				if (Player.usingSword()) Player.SetWeaponThrown(false);
				else Player.SetBoomerangComingBack();
				vOctorok[i]->SetHurt(false);
			}
		}
	}

	for (int i = 0; i < vTektike.size(); i++) {
		if (vTektike[i]->GetAlive()) {
			vTektike[i]->Logic(Scene.GetMap(level), &pchb, &whb, &dshb, playersWeaponThrown, directAttack, underSpell);
			if (vTektike[i]->GetHit()) {
				if (!Player.GetImmune()) {
					Player.Hurt();
					Sound.Play(HURT, EFFECTS_CHANNEL1);
				}
				vTektike[i]->SetHit(false);
			}
			if (vTektike[i]->GetHurt()) {
				Sound.Play(KILL, EFFECTS_CHANNEL1);
				int x, y;
				vTektike[i]->GetPosition(&x, &y);
				newExplosion(x, y); 
				if (Player.usingSword()) Player.SetWeaponThrown(false);
				else Player.SetBoomerangComingBack();
				vTektike[i]->SetHurt(false);
			}
		}
	}

	for (int i = 0; i < vWizzrobe.size(); i++) {
		if (vWizzrobe[i]->GetAlive()) {
			vWizzrobe[i]->Logic(Scene.GetMap(level), px, py, &pchb, &whb, &dshb, playersWeaponThrown, directAttack, underSpell);
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
				int x, y;
				vWizzrobe[i]->GetPosition(&x, &y);
				newExplosion(x, y);
				if (Player.usingSword()) Player.SetWeaponThrown(false);
				else Player.SetBoomerangComingBack();
				vWizzrobe[i]->SetHurt(false);
			}
		}
	}

	if (Aquamentus.GetAlive()) {
		Aquamentus.Logic(Scene.GetMap(level), &pchb, &whb, &dshb, playersWeaponThrown, directAttack, underSpell);
		if (Aquamentus.GetWeaponHit() || Aquamentus.GetHit()) {
			if (!Player.GetImmune()) {
				Player.Hurt();
				if (Aquamentus.GetWeaponHit()) Aquamentus.SetWeaponHit(false);
				Sound.Play(HURT, EFFECTS_CHANNEL1);
			}
			Aquamentus.SetHit(false);
		}
		if (Aquamentus.GetHurt()) {
			if (!Aquamentus.GetAlive()) {
				Sound.Play(AQUAMENTUS_KILLED, EFFECTS_CHANNEL1);
				int x, y;
				Aquamentus.GetPosition(&x, &y);
				newExplosion(x, y);
			}
			else Sound.Play(HIT, EFFECTS_CHANNEL1);
			if (Player.usingSword()) Player.SetWeaponThrown(false);
			else Player.SetBoomerangComingBack();
			Aquamentus.SetHurt(false);
		}
	}

	Player.countTime();

	for (int i = 0; i < explosions.size(); i++) {
		if (!explosions[i].stop) {
			explosions[i].e1.expand(0);
			explosions[i].e2.expand(1);
			explosions[i].e3.expand(2);
			explosions[i].e4.expand(3);
			++explosions[i].distance;
			if (explosions[i].distance > 25) explosions[i].stop = true;
		}
	}
}

void cGame::newExplosion(int x, int y) {
	cExplosion *part = new cExplosion();
	part->SetPosition(x, y);
	explosion e = {*part, *part, *part, *part, 0, false};
	explosions.push_back(e);
}

bool cGame::shieldProtected(int x, int y, int state)
{
	int px, py;
	Player.GetPosition(&px, &py);
	bool shield = false;
	switch (Player.GetState())
	{
	case STATE_LOOKLEFT:
	case STATE_WALKLEFT:	if (state % 4 == 1 && px < x+12) shield = true;
		break;
	case STATE_LOOKRIGHT:
	case STATE_WALKRIGHT:	if (state % 4 == 0 && px > x-12) shield = true;
		break;
	case STATE_LOOKUP:
	case STATE_WALKUP:		if (state % 4 == 3 && py < y+12) shield = true;
		break;
	case STATE_LOOKDOWN:
	case STATE_WALKDOWN:	if (state % 4 == 2 && py > y-12) shield = true;
		break;
	}

	return shield;
}
