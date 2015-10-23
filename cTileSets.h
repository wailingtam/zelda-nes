#pragma once
#include "TMXParser.h"
#include <utility>


class cTileSets{
public:
	cTileSets(void);
	virtual ~cTileSets(void);

	void init(TMX::Parser tmxa);

	 std::pair<float, float> get_texture_positions(int gid);

	 TMX::Parser::Tileset get_tileset(int gid);

	 std::pair<float, float> get_normalized_tile_size(int gid);

	 TMX::Parser tmx;

private:

	

};