#pragma once
#include "cTexture.h"
#include <iostream>

#define HEART_WIDTH 32/2 //In game, not texture
#define HEART_HEIGHT 28/2

#define MINIMAP_WIDTH 25
#define MINIMAP_HEIGHT 18

class cInterface {
public:
	cInterface(void);
	virtual ~cInterface(void);
	void init();

	void genList(float normalizedWith, int x, int y);

	void drawLive(int tex_id, float lives, int w);

	void drawMinimap(int tex_id, float normalizedPlayerx, float normalizedPlayery);




private:
	int oldTime = glutGet(GLUT_ELAPSED_TIME);
	int paintBlackDot = true;
};
