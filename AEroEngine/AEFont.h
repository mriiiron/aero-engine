#pragma once

#include <string>
#include "AEUtility.h"

using namespace std;


class AEFont {

public:

	GLvoid build();
	GLvoid raze();
	GLint getOffsetByChar(char ch);
	GLint getTexture() { return tex; }
	GLint getWidth() { return fontW; }
	GLint getHeight() { return fontH; }
	GLint getListBase() { return listBase; }
	GLvoid writeAt(GLint x, GLint y, const char* _Format, ...);
	GLvoid writeColoredAt(GLint x, GLint y, GLfloat r, GLfloat g, GLfloat b, const char* _Format, ...);
	GLvoid setWidth(GLint _width) { fontW = _width; }
	GLvoid setHeight(GLint _height) { fontH = _height; }
	GLvoid setTexture(GLint _tex) { tex = _tex; }
	GLvoid setName(string _name) { name == _name; }
	AERect getTexCoords(GLint imgOffset);

private:

	GLint tex;
	GLint listBase;
	GLint fontW, fontH;
	string name;

};


class AEFontLibrary {

public:

	static const GLint MAX_FONT_COUNT			= 30;
	
	AEFontLibrary();
	GLvoid add(AEFont* font);
	AEFont* getFont(GLint index) { return lib[index]; }

private:

	AEFont* lib[MAX_FONT_COUNT];
	GLint maxIndex;

};