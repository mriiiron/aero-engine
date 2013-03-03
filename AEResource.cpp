#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <GL\glut.h>
#include "AEImageLoader.h"
#include "AEUtility.h"
#include "AEResource.h"

extern char* context;

AERect AEResource::getTexCoords(GLint imgOffset, GLint imgCells) {
	AERect rect;
	switch (type) {
	case RES_1x1:
		rect.x1 = 0.0;
		rect.x2 = 1.0;
		rect.y2 = 1.0;
		rect.y1 = 0.0;
		break;
	case RES_1x5:
		rect.x1 = 0.2 * (imgOffset % 5);
		rect.x2 = rect.x1 + 0.2 * imgCells;
		rect.y2 = 1.0;
		rect.y1 = 0.0;
		break;
	case RES_2x5:
		rect.x1 = 0.2 * (imgOffset % 5);
		rect.x2 = rect.x1 + 0.2 * imgCells;
		rect.y2 = 1.0 - 0.5 * (imgOffset / 5);
		rect.y1 = rect.y2 - 0.5;
		break;
	case RES_5x10:
		rect.x1 = 0.1 * (imgOffset % 10);
		rect.x2 = rect.x1 + 0.1 * imgCells;
		rect.y2 = 1.0 - 0.2 * (imgOffset / 10);
		rect.y1 = rect.y2 - 0.2;
		break;
	default:
		break;
	}
	return rect;
}

AEResourceTable::AEResourceTable() {
	for (int i = 0; i < MAX_RESOURCE_COUNT; i++) {
		table[i] = NULL;
	}
}

GLvoid AEResourceTable::addAt(GLint index, AEResource* res) {
	if (index > MAX_RESOURCE_COUNT || index < 0) {
		// Error
		return;
	}
	table[index] = res;
}