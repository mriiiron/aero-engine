#pragma once

#include <string>
#include "AEFont.h"

using namespace std;

typedef struct _Text {

	AEFont* font;
	string text;
	GLint x, y;

} Text;


class AEHeadUpDisplay {

public:

	static const GLint MAX_TEXT_COUNTS			= 30;

	AEHeadUpDisplay();
	GLvoid addText(GLint x, GLint y, GLint fontIndex, string text);
	GLvoid update();
	GLvoid paint();

private:

	Text texts[MAX_TEXT_COUNTS];
	GLint maxTextIndex;

};