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
	TMX::Parser tmx;
	tmx.load(FILENAME);
	int scene_height = tmx.mapInfo.height;
	int scene_width = tmx.mapInfo.width;
	int tile_width = tmx.mapInfo.tileWidth;
	int tile_height = tmx.mapInfo.tileHeight;
	std::map<std::string, int> map();
	int gl_list = 0;
	cTileSets::init(tmx);
	//todo we try with just one layer first...

	for (std::map<std::string, TMX::Parser::TileLayer>::iterator it = tmx.tileLayer.begin(); it != tmx.tileLayer.end(); ++it) {	//for all layers
		glNewList(++gl_list, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, tex_id);
		glBegin(GL_QUADS);
		std::vector<std::vector<int> > layer_tiles = generate_matrix(tmx.tileLayer[it->first].data.contents, scene_height, scene_width);
		for (int i = layer_tiles.size() - 1; i > 0; i--) {
			for (int j = 0; j < layer_tiles[0].size(); j++) {	//for all the tilelayer (the vector holds the GID of the tiles)
				//todo: get first gid of the texture
				int gid = layer_tiles[i][j];
				std::pair<float, float> initial_pos = cTileSets::get_texture_positions(gid); //1. x, 2. y
				std::pair<float, float> tile_sizes = cTileSets::get_normalized_tile_size(gid);  //1. width, 2.height. We need to get values from 0 to 1.
				glTexCoord2f(initial_pos.first, initial_pos.second + tile_sizes.second); glVertex2i(px, py);	//todo: how to compute px, py
				glTexCoord2f(initial_pos.first + tile_sizes.first, initial_pos.second + tile_sizes.second); glVertex2i(px + BLOCK_SIZE, py);
				glTexCoord2f(initial_pos.first + tile_sizes.first, initial_pos.second); glVertex2i(px + BLOCK_SIZE, py + BLOCK_SIZE);
				glTexCoord2f(initial_pos.first, initial_pos.second); glVertex2i(px, py + BLOCK_SIZE);
			}
		}
	}
}


/// Generates a matrix representing the tiles of a layer
std::vector<std::vector<int> > cScene::generate_matrix(std::string const & s, int scene_height, int scene_width) {
	std::vector<std::string> v = explode(s, ',');
	std::vector<std::vector<int> > matrix(scene_height, std::vector<int>(scene_width));
	int i = 0;
	int j = 0;
	for (int pos = 0; pos < v.size(); pos++){
		++j;
		if (j == scene_width) ++i;
		j = j%scene_width;
		matrix[i][j] = atoi(v[pos].c_str());
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





bool cScene::LoadLevel(int level)
{
	bool res;
	FILE *fd;
	char file[16];
	int i,j,px,py;
	char tile;
	float coordx_tile, coordy_tile;

	res=true;

	




	if(level<10) sprintf(file,"%s0%d%s",(char *)FILENAME,level,(char *)FILENAME_EXT);
	else		 sprintf(file,"%s%d%s",(char *)FILENAME,level,(char *)FILENAME_EXT);

	fd=fopen(file,"r");
	if(fd==NULL) return false;

	id_DL=glGenLists(1);
	glNewList(id_DL,GL_COMPILE);	//Genero una display list (textura emmagatzemada a la targeta gràfica que cData ha guardat, utilitzo el id_ID per obtenir-ho)
		glBegin(GL_QUADS);
	
			for(j=SCENE_HEIGHT-1;j>=0;j--)
			{
				px=SCENE_Xo;
				py=SCENE_Yo+(j*TILE_SIZE);

				for(i=0;i<SCENE_WIDTH;i++)
				{
					fscanf(fd,"%c",&tile);
					if(tile==' ')
					{
						//Tiles must be != 0 !!!
						map[(j*SCENE_WIDTH)+i]=0;
					}
					else
					{
						//Tiles = 1,2,3,...
						map[(j*SCENE_WIDTH)+i] = tile-48; //De caràcter a enter

						if(map[(j*SCENE_WIDTH)+i]%2) coordx_tile = 0.0f;
						else						 coordx_tile = 0.5f;
						if(map[(j*SCENE_WIDTH)+i]<3) coordy_tile = 0.0f;
						else						 coordy_tile = 0.5f;

						//Coordenades d'imatge: BLOCK_SIZE = 24, FILE_SIZE = 64 (tamany de la textura dels blocs)
						// 24 / 64 = 0.375
						glTexCoord2f(coordx_tile       ,coordy_tile+0.375f);	glVertex2i(px           ,py           );
						glTexCoord2f(coordx_tile+0.375f,coordy_tile+0.375f);	glVertex2i(px+BLOCK_SIZE,py           );
						glTexCoord2f(coordx_tile+0.375f,coordy_tile       );	glVertex2i(px+BLOCK_SIZE,py+BLOCK_SIZE);
						glTexCoord2f(coordx_tile       ,coordy_tile       );	glVertex2i(px           ,py+BLOCK_SIZE);
					}
					px+=TILE_SIZE;
				}
				fscanf(fd,"%c",&tile); //pass enter
			}

		glEnd();
	glEndList();	//Deixo de pintar

	fclose(fd);

	return res;
}

void cScene::Draw(int tex_id)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glCallList(id_DL);
	glDisable(GL_TEXTURE_2D);
}
int* cScene::GetMap()
{
	return map;
}