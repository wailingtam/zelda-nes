#pragma once

#include "cTexture.h"
#include "Globals.h"
#include <array>
#include <iostream>

//Image array size
#define NUM_IMG		5

//Image identifiers
#define IMG_BLOCKS	1
#define IMG_PLAYER	2
#define IMG_ENEMIES 3
#define IMG_HEART   4
#define IMG_MINIMAP 5
/*
#define IMG_ENEMY1	2
#define IMG_ENEMY2	3
#define IMG_SHOOT	4
...
*/


class cData
{
public:
	cData(void);
	~cData(void);

	int  GetID(int img);
	void GetSize(int img,int *w,int *h);
	void loadImage(int img, char * filename, int type = GL_RGBA);
	void loadOverworldTextures();
	int * getOverworldIds();
	int loadImage(int img,char *filename, cTexture* textures, int type = GL_RGBA);

	

private:
	cTexture texture[NUM_IMG];
	//std::array<char*, 2> overworldTexturesFilenames = { "resources/tileset/grass-tiles-2-small.png",  "resources/tileset/tree2-final.png" };
	std::array<char*, 1> overworldTexturesFilenames = { "resources/tileset/roguelikeSheet_transparent.png"};
	//std::array<char*, 2> overworldTexturesFilenames = { "resources/tileset/zelda_tiles1.png" };
	cTexture overworldTextures[1];
	int textures[1];
};
