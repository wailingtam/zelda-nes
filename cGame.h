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
#include <vector>
#include <map>

#define GAME_WIDTH	800
#define GAME_HEIGHT 600
#define ZOOM_FACTOR 1

struct respawnOfBichos{
	std::vector<position> octorocks, tektikes, wizzrobes;
	position aquamentus;
};

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

	void spawn(/*espawnOfBichos * respawn*/);
	void soundsLoading();
	void gameLogic(int level);

	std::vector<cOctorok*> vOctorok;
	std::vector<cTektike*> vTektike;
	std::vector<cWizzrobe*> vWizzrobe;

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
	int w;
};
