#include "cTileSets.h"

cTileSets::cTileSets() {}
cTileSets::~cTileSets() {}

void cTileSets::init(TMX::Parser *tmxa) {
	this->tmx = tmxa;
}

///Given a gid, it returns the top-left x,y texture (or tileset) position, in value of 0 <-> 1
std::pair<float, float> cTileSets::get_texture_positions(int gid) {
	TMX::Parser::Tileset* actual_tileset = this->get_tileset(gid);	
	int tilewidth = this->tmx->mapInfo.tileWidth;	//We suppose that the size of the tile in the texture (tileset) is the same as in the tile layer
	int tileheight = this->tmx->mapInfo.tileHeight;
	int tiles_per_row = actual_tileset->imgwidth / tilewidth;
	int tiles_per_column = actual_tileset->imgheight / tileheight;
	int pos = gid - actual_tileset->firstGID;
	int row_pixels = (pos%tiles_per_row) * tilewidth;
	int column_pixels = (pos / tiles_per_row) * tileheight;


	//int row = tiles_per_row / pos;
	//int column = tiles_per_row % pos;
	float normalized_row = float(row_pixels) / float(tiles_per_row*tilewidth);
	float normalized_column = float(column_pixels) / float(tiles_per_column*tileheight);
	return std::make_pair(normalized_row, normalized_column);
}

TMX::Parser::Tileset* cTileSets::get_tileset(int gid) {
	for (int i = 0; i < this->tmx->tilesetList.size(); i++) {
		if (gid < tmx->tilesetList[i].firstGID) return &tmx->tilesetList[i-1];
	}
	return &tmx->tilesetList[tmx->tilesetList.size() - 1];
}

std::pair<float, float> cTileSets::get_normalized_tile_size(int gid) {
	TMX::Parser::Tileset* actual_tileset = this->get_tileset(gid);
	return std::make_pair((float)this->tmx->mapInfo.tileWidth / (float)actual_tileset->imgwidth, (float)this->tmx->mapInfo.tileHeight / (float)actual_tileset->imgheight);
}