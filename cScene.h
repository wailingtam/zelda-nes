#pragma once

#include "cTexture.h"
#include "cTileSets.h"
#include <iostream>

#define SCENE_Xo		(2*TILE_SIZE)
#define SCENE_Yo		TILE_SIZE
#define SCENE_WIDTH		16
#define SCENE_HEIGHT	16


#define FILENAME		"level"
#define FILENAME_EXT	".txt"

#define TILE_SIZE		16
#define BLOCK_SIZE		16

#define OVERWORLD_FILENAME	"resources/overworld.tmx"
#define OVERWORLD_LEVEL 0
#define INNERWORLD_LEVEL 1
#define INNERWORLD_FILENAME "resources/innerworld.tmx"



struct position {
	int x;
	int y;
};

struct changeLevelPosition {
	position overworld;
	position innerworld;
};

struct square {
	int gid;
	bool blocking;
	bool changeLevel;
	position actualpos;
	position newpos;
};

typedef std::vector<std::vector<square> > worldMatrix;


class cScene
{
public:
	cScene(void);
	virtual ~cScene(void);

	void loadOverworld();
	void loadInnerworld();
	
	std::vector<std::vector<int>> generate_matrix(std::vector<int> tiles, int scene_height, int scene_width);

	std::vector<std::string> explode(std::string const & s, char delim);

	///Ordered as the layers
	void Draw(int * texs_id);
	worldMatrix* GetMap(const int level);

	void setDrawing(int level);


private:
	void loadLevel(unsigned int level);
	void setPosInMap(worldMatrix * map, cTileSets * ctiles, const int i, const int j, const int level, const int gid);
	
	worldMatrix overworldMap, innerworldMap;	//scene
	int id_DL;								//actual level display list
	int scene_height;
	int firstIdOfActualLists, numberOfLayers;
	int firstId[2];
	std::vector<changeLevelPosition> changepos = { //for array
		{	//for struct 1
			{
				{ 91 , 20 },	//Overworld: x, y
				{ 42 , 17 }		//Innerworld: x, y
			}
		}
	};
};
