#pragma once

#include "cScene.h"
#include "cPlayer.h"
#include "cData.h"

#define GAME_WIDTH	800
#define GAME_HEIGHT 600
#define ZOOM_FACTOR 4

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
	cData Data;
	bool isOverworld = true;
};
