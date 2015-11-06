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
	this->w = h;
}

bool cGame::Init()
{
	bool res=true;

	cGame::setView(GAME_WIDTH, GAME_HEIGHT);

	//Scene initialization
	
	Data.loadOverworldTextures();
	Scene.loadOverworld();
	Scene.loadInnerworld();

	//Player(Link) initialization
	Data.loadImage(IMG_PLAYER, "resources/tileset/greenlink2.png", GL_RGBA);
	Player.Init();

	Data.loadImage(IMG_ENEMIES, "resources/tileset/enemies.png", GL_RGBA);

	srand(time(NULL));

	Octorok.Init();
	Tektike.Init();
	Wizzrobe.Init();
	Aquamentus.Init();

	Data.loadImage(IMG_HEART, "resources/tileset/heart.png", GL_RGBA);
	Data.loadImage(IMG_MINIMAP, "resources/minimap.png", GL_RGBA);

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
	bool res = true;
	int level = this->isOverworld ? OVERWORLD_LEVEL : INNERWORLD_LEVEL;

	//Process Input
	if (keys[27])	res = false;

	if (keys[GLUT_KEY_UP])			Player.MoveUp(Scene.GetMap(level));
	else if (keys[GLUT_KEY_DOWN])	Player.MoveDown(Scene.GetMap(level));
	else if (keys[GLUT_KEY_LEFT])	Player.MoveLeft(Scene.GetMap(level));
	else if (keys[GLUT_KEY_RIGHT])	Player.MoveRight(Scene.GetMap(level));
	else if (keys[GLUT_KEY_F1]) this->isOverworld = !this->isOverworld;
	else Player.Stop();
	if (keys[97])					Player.Attack(Scene.GetMap(level));		//S = 115


																		//Game Logic
	cRect pchb = Player.GetCurrentHitbox();
	Player.Logic(Scene.GetMap(level), &pchb);
	pchb = Player.GetCurrentHitbox();
	cRect shb = Player.GetSwordHitbox();
	bool swordThrown = Player.GetWeaponThrown();
	bool hitPlayer = false;


	if (Octorok.GetAlive()) Octorok.Logic(Scene.GetMap(level), &pchb, &shb, swordThrown);
	Tektike.Logic(Scene.GetMap(level), &pchb, &shb, swordThrown);
	int px, py;
	Player.GetPosition(&px, &py);
	Wizzrobe.Logic(Scene.GetMap(level), px, py, &pchb, &shb, swordThrown);
	Aquamentus.Logic(Scene.GetMap(level), &pchb, &shb, swordThrown);

	if (Octorok.GetWeaponHit()) {
		if (!Player.GetImmune()) {
			Player.Hurt();
			Octorok.SetWeaponThrown(false);
		}
		Octorok.SetWeaponHit(false);
	}
	if (Octorok.GetHit() /*|| Tektike.GetHit() || Wizzrobe.GetHit() || Aquamentus.GetHit()*/) {
		if (!Player.GetImmune()) Player.Hurt();
		Octorok.SetHit(false);
		/*Tektike.SetHit(false);
		Wizzrobe.SetHit(false);
		Aquamentus.SetHit(false);*/
	}
	if (Octorok.GetHurt()) {
		Player.SetWeaponThrown(false);
		Octorok.SetHurt(false);
	}

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
		Player.Draw(Data.GetID(IMG_PLAYER));
		if (Octorok.GetAlive()) Octorok.Draw(Data.GetID(IMG_ENEMIES));
		Tektike.Draw(Data.GetID(IMG_ENEMIES));
		Wizzrobe.Draw(Data.GetID(IMG_ENEMIES));
		Aquamentus.Draw(Data.GetID(IMG_ENEMIES));
	glLoadIdentity();
		Interface.drawLive(Data.GetID(IMG_HEART), Player.GetLives(), this->w);
		if (this->isOverworld) {
			int x, y;
			Player.GetPosition(&x, &y);
			worldMatrix *map = Scene.GetMap(OVERWORLD_LEVEL);
			Interface.drawMinimap(Data.GetID(IMG_MINIMAP), (float)x / (float)((*map)[0].size() * BLOCK_SIZE), (float)y / (float)(map->size() * BLOCK_SIZE));
		}
	glutSwapBuffers();
}


void cGame::moveCamera() {
	int playerx, playery;
	Player.GetPosition(&playerx, &playery);
	glTranslatef(-playerx, -playery, 0);	//Move the camera with the player
}