#pragma once

#include "cTexture.h"
#include "Globals.h"
#include <array>
#include <iostream>

//Image array size
#define NUM_IMG		2

//Image identifiers
#define IMG_BLOCKS	0
#define IMG_PLAYER	1
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
	std::array<char*, 2> overworldTexturesFilenames = { "resources/tileset/grass-tiles-2-small.png",  "resources/tileset/tree2-final.png" };
	//std::array<char*, 2> overworldTexturesFilenames = { "resources/tileset/zelda_tiles1.png" };
	cTexture overworldTextures[2];
	int textures[2];
};
