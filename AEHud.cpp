#include <GL\glut.h>
#include "AEHud.h"

using namespace std;

extern AEFontLibrary fontLib;

AEHeadUpDisplay::AEHeadUpDisplay() {
	maxTextIndex = 0;
}

GLvoid AEHeadUpDisplay::addText(GLint x, GLint y, GLint fontIndex, string text) {
	texts[maxTextIndex].font = fontLib.getFont(fontIndex);
	texts[maxTextIndex].text = text;
	texts[maxTextIndex].x = x;  texts[maxTextIndex].y = y;
	maxTextIndex++;
}

GLvoid AEHeadUpDisplay::update() {

}

GLvoid AEHeadUpDisplay::paint() {
	for (GLint i = 0; i < maxTextIndex; i++) {
		texts[i].font->writeAt(texts[i].x, texts[i].y, texts[i].text.c_str());
	}
}