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

void cData::loadImage(int img, char *filename, cTexture* textures, int type)
{
	if (!textures[img].Load(filename, type)) throw cdataLoadImageError;
}

void cData::loadOverworldTextures() {
	int id = 0;
	for (auto it = this->overworldTexturesFilenames.begin(); it != this->overworldTexturesFilenames.end(); ++it) {
		this->loadImage(id++, (*it), this->overworldTextures);
	}
}

int* cData::getOverworldIds() {
	int textures[sizeof(this->overworldTextures)];
	int i = 0;
	for (cTexture* it = this->overworldTextures; it != this->overworldTextures; ++it) {
		textures[i++] = ((*it).GetID());
	}
	return textures;
}