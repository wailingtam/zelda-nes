#include "cScene.h"
#include "Globals.h"
#include <sstream>

/**
	Generates the scene.

	Interest links:
	- http://sabia.tic.udc.es/gc/Contenidos%20adicionales/trabajos/Imagenyvideo/Aplicacion%20de%20texturas/ejemplo.htm
	- http://gamedevelopment.tutsplus.com/tutorials/parsing-tiled-tmx-format-maps-in-your-own-game-engine--gamedev-3104
	- https://github.com/solar-storm-studios/TMXParser
*/

cScene::cScene(void){
/*	std::vector<int> ox = {91};
	int oy[] = { 20 };
	int ix[] = { 42 };
	int iy[] = { 17 };
	for (int i = 0; i < ox.size(); i++) {
		changeLevelPosition pos;
		pos.overworld.x = 91;
		pos.overworld.y = 20;
		pos.innerworld.x = ix[i];
		pos.innerworld.y = iy[i];
		changepos.push_back(pos);
	}*/
}

cScene::~cScene(void)
{
}

/*
	Generates the overworld map
*/
void cScene::loadOverworld() {
	this->loadLevel(OVERWORLD_LEVEL);
}

void cScene::loadInnerworld() {
	this->loadLevel(INNERWORLD_LEVEL);
}

///We load the level. 
void cScene::loadLevel(unsigned int level) {
	TMX::Parser tmx;
	tmx.load(level == OVERWORLD_LEVEL? OVERWORLD_FILENAME : INNERWORLD_FILENAME);
	int scene_height = tmx.mapInfo.height;
	int scene_width = tmx.mapInfo.width;
	int tile_width = tmx.mapInfo.tileWidth; //Tile width in the map, not in the tileset.
	int tile_height = tmx.mapInfo.tileHeight;
	cTileSets ctiles = cTileSets();
	ctiles.init(&tmx);

	this->numberOfLayers = tmx.tileLayer.size();
	
	//Initializer for the global map
	worldMatrix* map = (level == OVERWORLD_LEVEL) ? &this->overworldMap : &this->innerworldMap;
	map->resize(scene_height);
	for (int i = 0; i < scene_height; ++i) (*map)[i].resize(scene_width);

	int gl_list = this->firstId[level] = glGenLists(this->numberOfLayers);	//We generate the display lists for this level.
	for (std::map<std::string, TMX::Parser::TileLayer>::iterator it = tmx.tileLayer.begin(); it != tmx.tileLayer.end(); ++it) {	//for all layers
		glNewList(gl_list++, GL_COMPILE);	//We generate
		glBegin(GL_QUADS);
		std::vector<std::vector<int> > layer_tiles = generate_matrix(tmx.tileLayer[it->first].data.tiles, scene_height, scene_width);	
		int y = 0; //OpenGL coordinates, we start from 0, 0.
		for (int i = layer_tiles.size() - 1; i > 0; i--) {
			int x = 0;
			for (int j = 0; j < layer_tiles[0].size(); j++) {	//for all the tilelayer (the vector holds the GID of the tiles)
				int gid = layer_tiles[i][j];
				if (gid != 0) {	//If gid == 0 it means it's nothing. However, the tilesets start with gid == 0. So for tilesets we will need to do gid - 1
					std::pair<float, float> initial_pos = ctiles.get_texture_positions(gid); //1. x, 2. y
					std::pair<float, float> texture_tile_sizes = ctiles.get_normalized_tile_size(gid);  //1. width, 2.height. We need to get values from 0 to 1.
					glTexCoord2f(initial_pos.first, initial_pos.second + texture_tile_sizes.second); glVertex2i(x, y);
					glTexCoord2f(initial_pos.first + texture_tile_sizes.first, initial_pos.second + texture_tile_sizes.second); glVertex2i(x + tile_width, y);
					glTexCoord2f(initial_pos.first + texture_tile_sizes.first, initial_pos.second); glVertex2i(x + tile_width, y + tile_height);
					glTexCoord2f(initial_pos.first, initial_pos.second); glVertex2i(x, y + tile_height);

					if((*it).first != "object4" && (*it).first != "roof") this->setPosInMap(map, &ctiles, i, j, level, gid);
				}
				x += tile_width;
			}
			y += tile_height;
		}
		glEnd();
		glEndList();
	}
}

///sets in the global map if the position is blocking or there is a changelevel on it.
void cScene::setPosInMap(worldMatrix* map, cTileSets* ctiles, const int i, const int j, const int level, const int gid) {
	(*map)[i][j].actualpos = { j,i };
	for (changeLevelPosition changeLevel : this->changepos) {
		if (OVERWORLD_LEVEL == level && j == changeLevel.overworld.x && i == changeLevel.overworld.y) {
			(*map)[i][j].changeLevel = true;
			(*map)[i][j].newpos = changeLevel.innerworld;
		}
		else if (OVERWORLD_LEVEL != level && j == changeLevel.innerworld.x && i == changeLevel.innerworld.y) {
			(*map)[i][j].changeLevel = true;
			(*map)[i][j].newpos = changeLevel.overworld;
		}
	}
	TMX::Parser::Tileset* tileset = (*ctiles).get_tileset(gid);
	auto it = tileset->property.find(gid - 1); //for the tilesets gid is the actual one -1 
	if (it != tileset->property.end()) {
		auto it2 = it->second.find("blocking");
		if (it2 != it->second.end()) {
			(*map)[i][j].blocking = true;
		}
	}
}


/// Generates a matrix representing the tiles of a layer
std::vector<std::vector<int> > cScene::generate_matrix(std::vector<int> tiles, int scene_height, int scene_width) {
	
	std::vector<std::vector<int> > matrix(scene_height, std::vector<int>(scene_width));
	int i = 0;
	int j = 0;
	for (auto it = tiles.begin(); it != tiles.end(); ++it) {
		matrix[i][j] = (*it);
		++j;
		if (j == scene_width) {
			++i;
			j = 0;
		}		
	}
	return matrix;
}


std::vector<std::string> cScene::explode(std::string const & s, char delim)
{
	std::vector<std::string> result;
	std::istringstream iss(s);

	for (std::string token; std::getline(iss, token, delim); )
	{
		result.push_back(std::move(token));
	}

	return result;
}

void cScene::setDrawing(int level) {
	this->firstIdOfActualLists = this->firstId[level];
}


void cScene::Draw(int* texs_id){
	glEnable(GL_TEXTURE_2D);
	int gl_list = this->firstIdOfActualLists;
	for (int i = 0; i < this->numberOfLayers - 2; i++) {	
		glBindTexture(GL_TEXTURE_2D, texs_id[0]);
		glCallList(gl_list++);
	}
	glDisable(GL_TEXTURE_2D);
}
worldMatrix* cScene::GetMap(const int level){
	return (level == OVERWORLD_LEVEL) ? &this->overworldMap : &this->innerworldMap;
}

void cScene::DrawAboveBichos(int* texs_id) {
	glEnable(GL_TEXTURE_2D);
	int gl_list = this->firstIdOfActualLists + this->numberOfLayers - 2;
	for (int i = this->numberOfLayers - 2; i < this->numberOfLayers; i++) {
		glBindTexture(GL_TEXTURE_2D, texs_id[0]);
		glCallList(gl_list++);
	}
	glDisable(GL_TEXTURE_2D);
}

