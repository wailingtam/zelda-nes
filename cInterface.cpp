#include "cInterface.h"

cInterface::cInterface(void)
{
}

cInterface::~cInterface(void)
{
}

void cInterface::init(){
	
}

void cInterface::genList(float normalizedWith, int x, int y) {
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex2d(x, y);
		glTexCoord2f(normalizedWith, 1); glVertex2d(x + HEART_WIDTH*normalizedWith, y);
		glTexCoord2f(normalizedWith, 0); glVertex2d(x + HEART_WIDTH*normalizedWith, y + HEART_HEIGHT);
		glTexCoord2f(0, 0); glVertex2d(x, y + HEART_HEIGHT);
	glEnd();
}

void cInterface::drawLive(int tex_id, float lives, int w) {
	glPushMatrix();

	//int initialX = 60;
	//int gameHeight = 600;
	//double pos = (initialX * w) / gameHeight;

	glTranslated(MINIMAP_WIDTH + 5, MINIMAP_HEIGHT/2 - HEART_HEIGHT/2, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex_id);
	int i;
	for (i = 0; i < int(lives); i++) genList(1, i * HEART_WIDTH, 0);
	if (lives != int(lives)) genList(0.5, i * HEART_WIDTH, 0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void cInterface::drawMinimap(int tex_id, float normalizedPlayerx, float normalizedPlayery) {
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex_id);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 1); glVertex2d(0, 0);
			glTexCoord2f(1, 1); glVertex2d(MINIMAP_WIDTH, 0);
			glTexCoord2f(1, 0); glVertex2d(MINIMAP_WIDTH, MINIMAP_HEIGHT);
			glTexCoord2f(0, 0); glVertex2d(0, MINIMAP_HEIGHT);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
	int newTime = glutGet(GLUT_ELAPSED_TIME);
	if (newTime - this->oldTime >= 1000) {
		this->paintBlackDot = !this->paintBlackDot;
		this->oldTime = newTime;
	}
	if (this->paintBlackDot) {
		glPushMatrix();
		
		float playerXInMap = normalizedPlayerx * MINIMAP_WIDTH;
		float playerYInMap = normalizedPlayery * MINIMAP_HEIGHT;
		float padding = 1;

		glBegin(GL_QUADS);
			glColor3d(0, 0, 0);
			glVertex2f(playerXInMap, playerYInMap);
			glVertex2f(playerXInMap + padding, playerYInMap);
			glVertex2f(playerXInMap + padding, playerYInMap + padding);
			glVertex2f(playerXInMap, playerYInMap + padding);
		glEnd();
		glColor3d(1, 1, 1);
		glPopMatrix();
	}
}