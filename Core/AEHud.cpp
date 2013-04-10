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
	maxTextIndex++;
}

GLvoid AEHeadUpDisplay::addImage(GLint _x, GLint _y, AEResource* _res, GLint _imgOffset) {
	images[maxImageIndex] = new HUDImage;
	images[maxImageIndex]->res = _res;  images[maxImageIndex]->imgOffset = _imgOffset;
	images[maxImageIndex]->x = _x;  images[maxImageIndex]->y = _y;
	maxImageIndex++;
}

GLvoid AEHeadUpDisplay::update() {
	
}

GLvoid AEHeadUpDisplay::paint() {
	for (GLint i = 0; i < maxTextIndex; i++) {
		texts[i]->font->writeAt(texts[i]->x, texts[i]->y, texts[i]->text.c_str());
	}
	for (GLint i = 0; i < maxImageIndex; i++) {
		AEResource* res = images[i]->res;
		AEUtil::paintRectByCenter(res->getTexture(), res->getTexCoords(images[i]->imgOffset, 1), AEUtil::createPoint(images[i]->x, images[i]->y), res->getCellWidth(), res->getCellHeight(), 0);
	}
}