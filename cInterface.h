#pragma once
#include "cTexture.h"
#include <iostream>

#define HEART_WIDTH 32/4 //In game, not texture
#define HEART_HEIGHT 32/4

#define MINIMAP_WIDTH 25
#define MINIMAP_HEIGHT 18

#define SWORD_HEIGHT 48/4
#define SWORD_WIDTH 21/4
#define BOW_HEIGHT 32/4
#define BOW_WIDTH 32/4

#define MAGIC_HEIGHT 24/2
#define MAGIC_WIDTH 24/2
#define MARGIN 5

class cInterface {
public:
	cInterface(void);
	virtual ~cInterface(void);
	void init();

	void genList(float normalizedWith, int x, int y, int width, int height);

	void drawLive(int tex_id, float lives, int w);

	void drawWeapon(int tex_id, bool isSword);

	void drawMagic(int tex_id);

	void drawMinimap(int tex_id, float normalizedPlayerx, float normalizedPlayery);




private:
	int oldTime = glutGet(GLUT_ELAPSED_TIME);
	int paintBlackDot = true;
};
