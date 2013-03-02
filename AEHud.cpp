#include <GL\glut.h>
#include "AEFont.h"
#include "AEHud.h"

AEHeadUpDisplay::AEHeadUpDisplay() {
	maxTextIndex = 0;
}

GLvoid AEHeadUpDisplay::addText(GLint _x, GLint _y, Font _font) {
	texts[maxTextIndex].font = _font;
	texts[maxTextIndex].x = _x;  texts[maxTextIndex].y = _y;
	maxTextIndex++;
}

GLvoid AEHeadUpDisplay::paint() {
	for (GLint i = 0; i < maxTextIndex; i++) {
		texts[i].font.writeAt(texts[i].x, texts[i].y);
	}
}