#pragma once
#include "cBicho.h"
class cExplosion :
	public cBicho
{
public:
	cExplosion();
	~cExplosion();

	void expand(int part);

	void draw(int part, int tex_id);
};

