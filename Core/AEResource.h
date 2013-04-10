/**
 * @file	AEResource.h
 * @brief	Defining the resources being used.
 *
 * @author	CaiyZ (Mr.IroN)
 * @version	0.1
 * @date	2013
*/

#pragma once

#include "AEUtility.h"


typedef enum _ResType {
	RES_1x1, RES_1x5, RES_2x5, RES_4x5, RES_5x10, RES_FONT
} ResType;

class AEResource {

public:

	GLvoid setCellWidth(GLint _cellW) { cellW = _cellW; }
	GLvoid setCellHeight(GLint _cellH) { cellH = _cellH; }
	GLvoid setTexture(GLint _tex) { tex = _tex; }
	GLvoid setType(ResType _type) { type = _type; }
	GLint getTexture() { return tex; }
	GLint getCellWidth() { return cellW; }
	GLint getCellHeight() { return cellH; }
	AERect getTexCoords(GLint imgOffset, GLint imgCells);

private:
	
	GLint tex;
	ResType type;
	GLint cellW, cellH;

};


class AEResourceTable {

public:

	static const GLint MAX_RESOURCE_COUNT		= 150;

	AEResourceTable();
	GLvoid addAt(GLint index, AEResource* res);
	AEResource* get(GLint index) { return table[index]; }

private:

	AEResource* table[MAX_RESOURCE_COUNT];

};