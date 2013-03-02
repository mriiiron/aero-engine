#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <GL\glut.h>
#include "AEImageLoader.h"
#include "AEUtility.h"
#include "AEResource.h"
#include "AEFont.h"
#include "AEBackground.h"
#include "AEParticleSystem.h"

extern AEResourceTable rTable;
extern AEBackgroundLibrary bgLib;
extern AEFontLibrary fontLib;
extern AEParticleSystem ptclSys;

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

GLvoid AEResourceTable::loadResourcesFromFile(char* dataFileName) {
	std::ifstream fs(dataFileName);
	char line[MAX_CHAR_COUNT_IN_LINE];
	printf("*** LOADING TEXTURES .. ***\n");
	while (!fs.eof()) {
		fs.getline(line, sizeof(line));
		if (strcmp(line, "") == 0) {
			continue;
		}
		else if (strcmp(line, "#resource") == 0) {
			fs.getline(line, sizeof(line));
			while (strcmp(line, "#resource_end") != 0) {
				char* item = strtok_s(line, " ", &context);
				AEResource* res = new AEResource();
				GLint rid = atoi(item);
				item = strtok_s(NULL, " ", &context);
				if (strcmp(item, "1x1") == 0) {
					res->setType(RES_1x1);
				}
				else if (strcmp(item, "1x5") == 0) {
					res->setType(RES_1x5);
				}
				else if (strcmp(item, "2x5") == 0) {
					res->setType(RES_2x5);
				}
				else if (strcmp(item, "5x10") == 0) {
					res->setType(RES_5x10);
				}
				// else if ...
				else {
					printf("Error: Resource format code error.\n");
					return;
				}
				item = strtok_s(NULL, " ", &context); res->setCellWidth(atoi(item));
				item = strtok_s(NULL, " ", &context); res->setCellHeight(atoi(item));
				item = strtok_s(NULL, " ", &context); res->setTexture(loadPNGTexture(item));
				rTable.addAt(rid, res);
				fs.getline(line, sizeof(line));
			}
		}
		else if (strcmp(line, "#font") == 0) {
			fs.getline(line, sizeof(line));
			while (strcmp(line, "#font_end") != 0) {
				AEFont* font = new AEFont();
				char* item = strtok_s(line, " ", &context);  font->setName(item);
				item = strtok_s(NULL, " ", &context);  font->setWidth(atoi(item));
				item = strtok_s(NULL, " ", &context);  font->setHeight(atoi(item));
				item = strtok_s(NULL, " ", &context);  font->setTexture(loadPNGTexture(item));
				font->build();
				fontLib.add(font);
				fs.getline(line, sizeof(line));
			}
		}
		else if (strcmp(line, "#bg") == 0) {
			fs.getline(line, sizeof(line));
			while (strcmp(line, "#bg_end") != 0) {
				AEBackground* bg = new AEBackground();
				char* item = strtok_s(line, " ", &context);  bg->loadFromFile(item);
				bgLib.add(bg);
				fs.getline(line, sizeof(line));
			}
		}
		else if (strcmp(line, "#ptcl") == 0) {
			fs.getline(line, sizeof(line));
			while (strcmp(line, "#ptcl_end") != 0) {
				char* item = strtok_s(line, " ", &context);  GLint ptclTexIndex = atoi(item);
				item = strtok_s(NULL, " ", &context);
				ptclSys.loadTexture(ptclTexIndex, item);
				fs.getline(line, sizeof(line));
			}
		}
	}
	printf("*** END LOADING TEXTURES. ***\n\n");
	fs.close();
}

GLvoid AEResourceTable::addAt(GLint index, AEResource* res) {
	if (index > MAX_RESOURCE_COUNT || index < 0) {
		// Error
		return;
	}
	table[index] = res;
}