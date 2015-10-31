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
	/*res = Data.LoadImage(IMG_PLAYER,"bub.png",GL_RGBA);
	if(!res) return false;
	Player.SetWidthHeight(32,32);
	Player.SetTile(4,1);
	Player.SetWidthHeight(32,32);
	Player.SetState(STATE_LOOKRIGHT);*/

	//Player(Link) initialization
	Data.loadImage(IMG_PLAYER, "resources/tileset/greenlink.png", GL_RGBA);
	Player.SetWidthHeight(40, 40);
	Player.SetTile(4, 1);
	Player.SetState(STATE_LOOKDOWN);
	Player.SetLives(2);

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
	bool res=true;
	
	//Process Input, decidirà si en moc o no aun nou lloc
	if(keys[27])	res=false;
	
	/*if(keys[GLUT_KEY_UP])			Player.Jump(Scene.GetMap());
	if(keys[GLUT_KEY_LEFT])			Player.MoveLeft(Scene.GetMap());
	else if(keys[GLUT_KEY_RIGHT])	Player.MoveRight(Scene.GetMap());
	else Player.Stop();*/

	if (keys[GLUT_KEY_UP])			Player.MoveUp(Scene.GetMap());
	else if (keys[GLUT_KEY_DOWN])	Player.MoveDown(Scene.GetMap());
	else if (keys[GLUT_KEY_LEFT])	Player.MoveLeft(Scene.GetMap());
	else if (keys[GLUT_KEY_RIGHT])	Player.MoveRight(Scene.GetMap());
	else if (keys[97])				Player.Attack(Scene.GetMap());
	else if (keys[GLUT_KEY_F1]) this->isOverworld = !this->isOverworld;
	else Player.Stop();

	
	
	//Game Logic En aquest cas la logica es continuar l'ordre que ha fet l'usuari: o continuo saltant o continuo caient. 
	Player.Logic(Scene.GetMap());

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

	glutSwapBuffers();
}


void cGame::moveCamera() {
	int playerx, playery;
	Player.GetPosition(&playerx, &playery);
	glTranslatef(-playerx, -playery, 0);	//Move the camera with the player
}