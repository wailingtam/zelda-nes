#pragma once
#include "TMXParser.h"
#include <utility>


class cTileSets{
public:

	static void init(TMX::Parser tmx);

	static std::pair<float, float> get_texture_positions(int gid);

	static TMX::Parser::Tileset get_tileset(int gid);

	static std::pair<float, float> get_normalized_tile_size(int gid);

private:

	static TMX::Parser tmx;

};