#pragma once

#include "AEFont.h"
#include "AEResource.h"
#include <string>

using namespace std;


typedef struct _HUDText {
	AEFont* font;
	string text;
	GLint x, y;
	GLclampf alpha;
} HUDText;


typedef struct _HUDImage {
	AEResource* res;
	GLint imgOffset;
	GLint x, y;
	GLclampf alpha;
} HUDImage;


class AEHeadUpDisplay {

public:

	static const GLint MAX_TEXT_COUNT			= 30;
	static const GLint MAX_IMAGE_COUNT			= 30;

	AEHeadUpDisplay();
	GLvoid addText(GLint _x, GLint _y, GLint fontIndex, string text);
	GLvoid addImage(GLint _x, GLint _y, AEResource* _res, GLint _imgOffset);
	GLvoid update();
	GLvoid paint();

private:

	HUDText* texts[MAX_TEXT_COUNT];
	HUDImage* images[MAX_IMAGE_COUNT];
	GLint maxTextIndex, maxImageIndex;

};