#pragma once

#include "cTexture.h"
#include "cTileSets.h"
#include <iostream>

#define SCENE_Xo		(2*TILE_SIZE)
#define SCENE_Yo		TILE_SIZE
#define SCENE_WIDTH		36
#define SCENE_HEIGHT	28


#define FILENAME		"level"
#define FILENAME_EXT	".txt"

#define TILE_SIZE		16
#define BLOCK_SIZE		24

#define OVERWORLD_FILENAME	"resources/overworld.tmx"

class cScene
{
public:
	cScene(void);
	virtual ~cScene(void);

	void loadOverworld();
	
	std::vector<std::vector<int>> generate_matrix(std::string const & s, int scene_height, int scene_width);


	std::vector<std::string> explode(std::string const & s, char delim);

	///Ordered as the layers
	void Draw(int * texs_id);
	int *GetMap();

private:
	void loadLevel(const char * filename);
	int map[SCENE_WIDTH * SCENE_HEIGHT];	//scene
	int id_DL;								//actual level display list
	int scene_height;
	int firstIdOfActualLists, numberOfLayers;
};
