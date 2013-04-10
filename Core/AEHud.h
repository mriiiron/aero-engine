/**
 * @file	AEHud.h
 * @brief	Defining the elements used by head-up displays.
 *
 * The HUDs is included in Scenes. Elements of HUD will not move along with camera movement.
 * You may create a relatively static scene only using HUD. For example, your game title screen.
 *
 * @author	CaiyZ (Mr.IroN)
 * @version	0.1
 * @date	2013
*/

#pragma once

#include "AEFont.h"
#include "AEResource.h"
#include <string>

using namespace std;


typedef struct _HUDText {
	AEFont* font;
	string text;
	GLint x, y;
} HUDText;


typedef struct _HUDImage {
	AEResource* res;
	GLint imgOffset;
	GLint x, y;
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