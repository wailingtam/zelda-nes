#include "cData.h"
#include <exception>

class cDataLoadImageError : public std::exception
{
	virtual const char* what() const throw()
	{
		return "cData: Image could not be loaded.";
	}
} cdataLoadImageError;

cData::cData(void) {}
cData::~cData(void){}

int cData::GetID(int img)
{
	return texture[img].GetID();
}

void cData::GetSize(int img, int *w, int *h)
{
	texture[img].GetSize(w,h);
}

void cData::loadImage(int img, char *filename, int type){
	this->loadImage(img, filename, texture, type);
}

int cData::loadImage(int img, char *filename, cTexture* textures, int type)
{
	if (!textures[img].Load(filename, type)) throw cdataLoadImageError;
	return textures[img].GetID();
}

void cData::loadOverworldTextures() {
	for (int i = 0; i < this->overworldTexturesFilenames.size(); i++) {
		textures[i] = this->loadImage(i, this->overworldTexturesFilenames[i], this->overworldTextures);
	}
}

int* cData::getOverworldIds() {
	return textures;
}