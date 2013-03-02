#pragma once

typedef enum _ResType {
	RES_1x1, RES_1x5, RES_2x5, RES_5x10, RES_FONT
} ResType;

class AEResource {
private:
	GLint tex;
	ResType type;
	GLint cellW, cellH;
public:
	GLvoid setCellWidth(GLint _cellW) { cellW = _cellW; }
	GLvoid setCellHeight(GLint _cellH) { cellH = _cellH; }
	GLvoid setTexture(GLint _tex) { tex = _tex; }
	GLvoid setType(ResType _type) { type = _type; }
	GLint getTexture() { return tex; }
	GLint getCellWidth() { return cellW; }
	GLint getCellHeight() { return cellH; }
	AERect getTexCoords(GLint imgOffset, GLint imgCells);
};

class AEResourceTable {

public:

	static const GLint MAX_RESOURCE_COUNT		= 150;

	AEResourceTable();
	GLvoid addAt(GLint index, AEResource* res);
	GLvoid loadResourcesFromFile(char* dataFileName);
	AEResource* get(GLint index) { return table[index]; }

private:

	AEResource* table[MAX_RESOURCE_COUNT];

};