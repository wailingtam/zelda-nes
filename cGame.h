#pragma once

#include "cScene.h"
#include "cPlayer.h"
#include "cOctorok.h"
#include "cTektike.h"
#include "cWizzrobe.h"
#include "cAquamentus.h"
#include "cData.h"
#include <vector>

#define GAME_WIDTH	800
#define GAME_HEIGHT 600
#define ZOOM_FACTOR 8

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

private:
	unsigned char keys[256];
	cScene Scene;
	cPlayer Player;
	cOctorok Octorok;
	cTektike Tektike;
	cWizzrobe Wizzrobe;
	cAquamentus Aquamentus;
	cData Data;
	bool isOverworld = true;
};
