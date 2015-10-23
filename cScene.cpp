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

cScene::cScene(void)
{

}

cScene::~cScene(void)
{
}

/*
	Generates the overworld map
*/
void cScene::loadOverworld() {
	this->loadLevel("resources/overworld.tmx");
}

///We load the level. 
void cScene::loadLevel(const char * filename) {
	TMX::Parser tmx;
	tmx.load(filename);  //todo we are going to have 2 files: overworld and underworld.
	int scene_height = tmx.mapInfo.height;
	int scene_width = tmx.mapInfo.width;
	int tile_width = tmx.mapInfo.tileWidth; //Tile width in the map, not in the tileset.
	int tile_height = tmx.mapInfo.tileHeight;
	std::map<std::string, int> map();
	cTileSets ctiles = cTileSets();
	ctiles.init(tmx);

	this->numberOfLayers = tmx.tileLayer.size();

	int gl_list = this->firstIdOfActualLists = glGenLists(this->numberOfLayers);	//We generate the display lists for this level.
	for (std::map<std::string, TMX::Parser::TileLayer>::iterator it = tmx.tileLayer.begin(); it != tmx.tileLayer.end(); ++it) {	//for all layers
		glNewList(gl_list++, GL_COMPILE);	//We generate
		glBegin(GL_QUADS);
		std::vector<std::vector<int> > layer_tiles = generate_matrix(tmx.tileLayer[it->first].data.tiles, scene_height, scene_width);	
		int y = 0; //OpenGL coordinates, we start from 0, 0.
		for (int i = layer_tiles.size() - 1; i > 0; i--) {
			int x = 0;
			for (int j = 0; j < layer_tiles[0].size(); j++) {	//for all the tilelayer (the vector holds the GID of the tiles)
				int gid = layer_tiles[i][j];
				if (gid != 0) {	//If gid == 0 it means it's nothing
					std::pair<float, float> initial_pos = ctiles.get_texture_positions(gid); //1. x, 2. y
					std::pair<float, float> texture_tile_sizes = ctiles.get_normalized_tile_size(gid);  //1. width, 2.height. We need to get values from 0 to 1.
					glTexCoord2f(initial_pos.first, initial_pos.second + texture_tile_sizes.second); glVertex2i(x, y);
					glTexCoord2f(initial_pos.first + texture_tile_sizes.first, initial_pos.second + texture_tile_sizes.second); glVertex2i(x + tile_width, y);
					glTexCoord2f(initial_pos.first + texture_tile_sizes.first, initial_pos.second); glVertex2i(x + tile_width, y + tile_height);
					glTexCoord2f(initial_pos.first, initial_pos.second); glVertex2i(x, y + tile_height);
				}
				x += tile_width;
			}
			y += tile_height;
		}
		glEnd();
		glEndList();
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


void cScene::Draw(int* texs_id){
	
	int gl_list = this->firstIdOfActualLists;
	for (int i = 0; i < this->numberOfLayers; i++) {
		glEnable(GL_TEXTURE_2D);
		int id_textura = texs_id[0];
		glBindTexture(GL_TEXTURE_2D,id_textura);
		glCallList(gl_list++);
		glDisable(GL_TEXTURE_2D);
	}
	
}
int* cScene::GetMap()
{
	return map;
}