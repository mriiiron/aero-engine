#include <GL\glut.h>
#include "AEUtility.h"
#include "AEHud.h"

using namespace std;

extern AEFontLibrary fontLib;

AEHeadUpDisplay::AEHeadUpDisplay() {
	maxTextIndex = maxImageIndex = 0;
}

GLvoid AEHeadUpDisplay::addText(GLint _x, GLint _y, GLint fontIndex, string text) {
	texts[maxTextIndex] = new HUDText;
	texts[maxTextIndex]->font = fontLib.getFont(fontIndex);
	texts[maxTextIndex]->text = text;
	texts[maxTextIndex]->x = _x;  texts[maxTextIndex]->y = _y;
	texts[maxTextIndex]->alpha = 1.0;
	maxTextIndex++;
}

GLvoid AEHeadUpDisplay::addImage(GLint _x, GLint _y, AEResource* _res, GLint _imgOffset) {
	images[maxImageIndex] = new HUDImage;
	images[maxImageIndex]->res = _res;  images[maxImageIndex]->imgOffset = _imgOffset;
	images[maxImageIndex]->x = _x;  images[maxImageIndex]->y = _y;
	images[maxImageIndex]->alpha = 1.0;
	maxImageIndex++;
}

GLvoid AEHeadUpDisplay::update() {
	
}

GLvoid AEHeadUpDisplay::paint() {
	for (GLint i = 0; i < maxTextIndex; i++) {
		glColor4f(1.0, 1.0, 1.0, texts[i]->alpha);
		texts[i]->font->writeAt(texts[i]->x, texts[i]->y, texts[i]->text.c_str());
		glColor4f(1.0, 1.0, 1.0, 1.0);
	}
	for (GLint i = 0; i < maxImageIndex; i++) {
		glColor4f(1.0, 1.0, 1.0, images[i]->alpha);
		AEResource* res = images[i]->res;
		AEUtil::paintRectByCenter(res->getTexture(), res->getTexCoords(images[i]->imgOffset, 1), AEUtil::createPoint(images[i]->x, images[i]->y), res->getCellWidth(), res->getCellHeight(), 0); 
		glColor4f(1.0, 1.0, 1.0, 1.0);
	}
}