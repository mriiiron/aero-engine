#include <GL\glut.h>
#include <fstream>
#include "AESystemParam.h"
#include "AEImageLoader.h"
#include "AEResource.h"
#include "AEBackground.h"
#include "AEFont.h"
#include "AEParticleSystem.h"
#include "AEFileLoader.h"

using namespace std;

extern AEResourceTable rTable;
extern AEBackgroundLibrary bgLib;
extern AEFontLibrary fontLib;
extern AEParticleSystem ptclSys;

extern char* context;

GLvoid AEFileLoader::loadMainDataFromFile(char* dataFileName) {
	ifstream fs(dataFileName);
	char line[AESysParam::MAX_CHAR_COUNT_IN_LINE];
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