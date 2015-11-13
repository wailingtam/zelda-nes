#pragma once

#include "cScene.h"
#include "cPlayer.h"
#include "cOctorok.h"
#include "cTektike.h"
#include "cWizzrobe.h"
#include "cAquamentus.h"
#include "cData.h"
#include "cInterface.h"
#include "cSound.h"
#include "cExplosion.h"
#include <vector>
#include <map>

#define GAME_WIDTH	800
#define GAME_HEIGHT 600
#define ZOOM_FACTOR 1

struct respawnOfBichos{
	std::vector<position> octorocks, tektikes, wizzrobes;
	position aquamentus;
};

struct zone {
	std::vector<position> vertexs;
	respawnOfBichos respawn;
	//bool overworld;
};

struct explosion {
	cExplosion e1, e2, e3, e4;
	int distance;
	bool stop;
};

typedef std::vector<zone> zones;

class cGame
{
public:
	cGame(void);
	virtual ~cGame(void);

	void setView(int w, int h);

	bool Init();
	bool Loop();
	void Finalize();

	//Input
	void ReadKeyboard(unsigned char key, int x, int y, bool press);
	void ReadMouse(int button, int state, int x, int y);
	//Process
	bool Process();
	//Output
	void Render();

	void moveCamera();

	void spawn(int zone);
	int getNewSpanZone();
	void soundsLoading();
	void newExplosion(int x, int y);
	bool shieldProtected(int x, int y, int state);
	void gameLogic(int level);

	std::vector<cOctorok*> vOctorok;
	std::vector<cTektike*> vTektike;
	std::vector<cWizzrobe*> vWizzrobe;
	std::vector<explosion> explosions;

protected:
	cSound Sound;

private:
	unsigned char keys[256];
	cScene Scene;
	cPlayer Player;
	cAquamentus Aquamentus;
	cInterface Interface;
	cData Data;
	bool isOverworld = true;
	int w, h;
	int currentZone;
	int switchTime = glutGet(GLUT_ELAPSED_TIME);
	bool gameOver = false;
	zones respawnZones = {
		{ //Zone forest
			{  //Vertexs
				{8,67}, {55,67}, {54,42}, {8,42}
			},
			{	//Respawn
				{ { 27, 61 },{ 34, 63 },{ 38, 58 },{ 46, 55 },{45,47} }, //Octoroks
				{ {46, 62}, {32, 48} , {42,59}, {26,51}}, //Tektikes
				{  }, //Wizzrobes
			},
		//	true

		},
		{ //Zone village
			{  //Vertexs
				{ 11,42 },{ 63,42 },{ 63,20 },{ 11,20 }
			},
			{	//Respawn
				{ { 31, 38 }, {32, 32}, {44,36}, {51,31},{ 32,21 } }, //Octoroks
				{ {36,24}, {38,30}, {59,34} }, //Tektikes
				{ {50,39} }, //Wizzrobes
			},
		//	true

		},
		{ //Zone cementery
			{  //Vertexs
				{ 37,21 },{ 61,21 },{ 61,10 },{ 37,10 }
			},
			{	//Respawn
				{ { 48, 14 },{ 45, 14 },{ 46, 14 } }, //Octoroks
				{ { 57, 21 } }, //Tektikes
				{ { 42, 14 },{ 59, 17 } }, //Wizzrobes
			},
		//	true

		},
		{ //Zone castle
			{  //Vertexs
				{ 75,39 },{ 96,39 },{ 96,23 },{ 75,23 }
			},
			{	//Respawn
				{ { 80, 35 } }, //Octoroks
				{ { 83,28 },{ 86,28 } }, //Tektikes
				{ { 25, 38 },{ 46, 28 } }, //Wizzrobes
			},
		//	true
		},
		{ //Zone inner castle
			{  //Vertexs
				{ 41,49 },{ 75,49 },{ 75,19 },{ 41,19 }
			},
			{	//Respawn
				{ { 44, 35 },{ 50, 36 },{ 60, 30 } }, //Octoroks
				{ { 44, 28 },{ 49, 28 }, {67,32} }, //Tektikes
				{ { 46, 43 },{ 47, 43 },{ 57, 30 },{ 64, 32 } }, //Wizzrobes
				{66, 32}  //Aquamentus
			},
		//	false
		}
	};
};
