#include <iostream>
#include <sstream>
#include <fstream>
#include <GL\glut.h>
#include "AESystemParam.h"
#include "AEImageLoader.h"
#include "AEResource.h"
#include "AEObject.h"
#include "AEBackground.h"
#include "AEFont.h"
#include "AEParticleSystem.h"
#include "AEFileLoader.h"

using namespace std;

extern AEResourceTable rTable;
extern AEObjectTable oTable;
extern AEBackgroundLibrary bgLib;
extern AEFontLibrary fontLib;
extern AEParticleSystem ptclSys;

GLvoid AEFileLoader::loadBackground(AEBackground* bg, string bgDataFileName) {
	ifstream fs(bgDataFileName);
	if (fs.fail()) {
		cout << "Failed: Cannot open \"bg.txt\"." << endl;
		return;
	}
	string line, item;
	istringstream iss;
	GLint layerAdded = 0;
	while (!fs.eof()) {
		getline(fs, line);  iss.clear();  iss.str(line);
		switch (line[0]) {
		case '%':
			break;
		case '#':
			iss >> item;
			if (item == "#name") {
				iss >> item;
				bg->setName(item);
			}
			else if (item == "#layercount") {
				iss >> item;
				GLint layerCount = stoi(item);
				if (layerCount > AEBackground::MAX_LAYER_COUNT) layerCount = AEBackground::MAX_LAYER_COUNT;
				bg->setLayerCount(layerCount);
			}
			else if (item == "#landform") {
				iss >> item;
				GLint width, height;
				GLubyte* landformData = AEImageLoader::loadPNG(item.c_str(), &width, &height);
				bg->setWidth(width);  bg->setHeight(height);
				bg->loadLandforms(landformData, width, height);
			}
			else if (item == "#resources") {
				getline(fs, line);  iss.clear();  iss.str(line);
				while (line != "#endresources") {
					AEResource* res = new AEResource();
					iss >> item;  GLint rid = stoi(item);
					iss >> item;
					if (item == "1x1") {
						res->setType(RES_1x1);
					}
					else if (item == "1x5") {
						res->setType(RES_1x5);
					}
					else if (item == "2x5") {
						res->setType(RES_2x5);
					}
					else if (item == "4x5") {
						res->setType(RES_4x5);
					}
					else if (item == "5x10") {
						res->setType(RES_5x10);
					}
					// else if ...
					else {
						printf("Error: Resource format code error.\n");
						return;
					}
					iss >> item;  res->setCellWidth(stoi(item));
					iss >> item;  res->setCellHeight(stoi(item));
					iss >> item;  res->setTexture(AEImageLoader::loadPNGTexture(item.c_str()));
					bg->getBGResTable()->addAt(rid, res);
					getline(fs, line);  iss.clear();  iss.str(line);
				}
			}
			else if (item == "#anim") {
				AEBGLayerAnim lAnim;
				iss >> item;  GLint animIndex = stoi(item);
				getline(fs, line);  iss.clear();  iss.str(line);
				iss >> item;
				if (item == "#framecount") {
					iss >> item;  lAnim.setFrameCount(stoi(item));
				}
				else {
					// Error
				}
				getline(fs, line);  iss.clear();  iss.str(line);
				GLint frameNum = 0, endTime = 0;
				while (line != "#endanim") {
					iss >> item;  GLint rid = stoi(item);
					iss >> item;  GLint offset = stoi(item);
					iss >> item;  endTime += stoi(item);
					lAnim.setFrameImage(frameNum, rid, offset, bg->getBGResTable()->get(rid)->getCellWidth(), bg->getBGResTable()->get(rid)->getCellHeight());
					lAnim.setEndTime(frameNum, endTime);
					frameNum++;
					getline(fs, line);  iss.clear();  iss.str(line);
				}
				bg->addAnimAt(animIndex, lAnim);
			}
			else if (item == "#layer") {
				if (layerAdded == bg->getLayerCount())
					break;
				iss >> item;  GLint lyr_width = stoi(item);
				iss >> item;  GLint lyr_height = stoi(item);
				GLint rid, depth = 0;
				GLfloat offsetx = 0.0, offsety = 0.0;
				iss >> item;
				while (item != "end:") {
					if (item == "depth:") {
						iss >> item;  depth = stoi(item);
					}
					else if (item == "offsetx:") {
						iss >> item;  offsetx = GLfloat(stoi(item));
					}
					else if (item == "offsety:") {
						iss >> item;  offsety = GLfloat(stoi(item));
					}
					// else if ..
					iss >> item;
				}
				getline(fs, line);  iss.clear();  iss.str(line);
				iss >> item;
				if (item == "#res") {
					iss >> item;  rid = stoi(item);
				}
				else {
					// Error
				}
				AEBGLayer* newLayer = new AEBGLayer(rid, depth, lyr_width, lyr_height, offsetx, offsety);
				getline(fs, line);  iss.clear();  iss.str(line);
				while (line != "#endlayer") {
					iss >> item;
					if (item == "$animref") {
						AnimRef ref;
						ref.time = ref.frame = 0;
						iss >> item;  ref.animIndex = stoi(item);
						iss >> item;  ref.x = stoi(item);
						iss >> item;  ref.y = stoi(item);
						newLayer->addAnimRef(ref);
					}
					else {
						// Error
					}
					getline(fs, line);  iss.clear();  iss.str(line);
				}
				GLubyte inserted = 0;
				for (GLint i = 0; i < layerAdded; i++) {
					if (newLayer->getDepth() < bg->getLayer(i)->getDepth()) {
						for (GLint j = layerAdded - 1; j >= i; j--) {
							bg->setLayer(j + 1, bg->getLayer(j));
						}
						bg->setLayer(i, newLayer);
						inserted = 1;
						break;
					}
				}
				if (!inserted) {
					bg->setLayer(layerAdded, newLayer);
				}
				layerAdded++;
			}
			break;
		default:
			break;
		}
	}
	fs.close();
}

GLvoid AEFileLoader::loadParticleTex(AEParticleSystem* ps, GLint index, string texFileName) {
	if (index > MAX_PARTICLE_TYPES || index < 0) {
		// Error
		return;
	}
	ps->setTexture(index, AEImageLoader::loadPNGTexture(texFileName.c_str()));
}

GLvoid AEFileLoader::loadMainData(string dataFileName) {
	ifstream fs(dataFileName);
	if (fs.fail()) {
		cout << "Failed: Cannot open \"maindata.txt\"." << endl;
		return;
	}
	string line, item;
	istringstream iss;
	cout << "*** LOADING MAIN DATA .. ***" << endl;
	while (!fs.eof()) {
		getline(fs, line);
		if (line == "") {
			continue;
		}
		else if (line == "#resource") {
			getline(fs, line);  iss.clear();  iss.str(line);
			while (line != "#resource_end") {
				AEResource* res = new AEResource();
				iss >> item;  GLint rid = stoi(item);
				iss >> item;
				if (item == "1x1") {
					res->setType(RES_1x1);
				}
				else if (item == "1x5") {
					res->setType(RES_1x5);
				}
				else if (item == "2x5") {
					res->setType(RES_2x5);
				}
				else if (item == "4x5") {
					res->setType(RES_4x5);
				}
				else if (item == "5x10") {
					res->setType(RES_5x10);
				}
				// else if ...
				else {
					printf("Error: Resource format code error.\n");
					return;
				}
				iss >> item;  res->setCellWidth(stoi(item));
				iss >> item;  res->setCellHeight(stoi(item));
				iss >> item;  res->setTexture(AEImageLoader::loadPNGTexture(item.c_str()));
				rTable.addAt(rid, res);
				getline(fs, line);  iss.clear();  iss.str(line);
			}
		}
		else if (line == "#font") {
			getline(fs, line);  iss.clear();  iss.str(line);
			while (line != "#font_end") {
				AEFont* font = new AEFont();
				iss >> item;  font->setName(item);
				iss >> item;  font->setWidth(stoi(item));
				iss >> item;  font->setHeight(stoi(item));
				iss >> item;  font->setTexture(AEImageLoader::loadPNGTexture(item.c_str()));
				font->build();
				fontLib.add(font);
				getline(fs, line);  iss.clear();  iss.str(line);
			}
		}
		else if (line == "#bg") {
			getline(fs, line);  iss.str(line);
			while (line != "#bg_end") {
				AEBackground* bg = new AEBackground();
				iss >> item;  loadBackground(bg, item);
				bgLib.add(bg);
				getline(fs, line);  iss.clear();  iss.str(line);
			}
		}
		else if (line == "#ptcl") {
			getline(fs, line);  iss.str(line);
			while (line != "#ptcl_end") {
				iss >> item;  GLint ptclTexIndex = stoi(item);
				iss >> item;  loadParticleTex(&ptclSys, ptclTexIndex, item);
				getline(fs, line);  iss.clear();  iss.str(line);
			}
		}
	}
	cout << "*** END LOADING MAIN DATA ***\n" << endl;
	fs.close();
}

GLvoid AEFileLoader::loadObject(AEObject* obj, string objectName) {
	ifstream fs("res\\" + objectName + "\\data.txt");
	if (fs.fail()) {
		cout << "Failed: Cannot open \"data.txt\" for Object " << objectName << "." << endl;
		return;
	}
	string line, item;
	istringstream iss;
	while (!fs.eof()) {
		getline(fs, line);
		if (line == "") {
			continue;
		}
		switch (line.at(0)) {
		case '%':
			continue;
		case '#':
			iss.clear();  iss.str(line);
			iss >> item;
			if (item == "#name") {
				iss >> item;  obj->setName(item);
			}
			else if (item == "#type") {
				iss >> item;  obj->setType(stoi(item));
			}
			else if (item == "#action") {
				AEAnimation* newAnim;
				iss >> item;  GLint animIndex = stoi(item);
				iss >> item;  //newAnim->setName(item);
				iss >> item;  GLint animState = stoi(item);
				getline(fs, line);
				iss.clear();  iss.str(line);
				iss >> item;
				if (item == "#framecount") {
					iss >> item;  GLint frameCount = stoi(item);
					newAnim = new AEAnimation(frameCount);
					newAnim->setState(animState);
					iss >> item;
					if (item == "loop") {		
						newAnim->setLoop(GL_TRUE);
						iss >> item;  GLint ttl = stoi(item);
						newAnim->setTTL(ttl);
						if (ttl != -1) {
							getline(fs, line);
							iss.clear();  iss.str(line);
							iss >> item;
							if (item == "#loopover") {
								iss >> item;  newAnim->setNext(stoi(item));
							}
							else {
								// Error
							}
						}
						else {
							newAnim->setNext(-1);
						}
					}
					else if (item == "next") {
						newAnim->setLoop(GL_FALSE);
						iss >> item;
						newAnim->setNext(stoi(item));
					}
					else {
						// Error
					}
				}
				else {
					// Error
				}
				GLint frameNum = 0, endTime = 0;
				getline(fs, line);
				while (line != "#endaction") {
					iss.clear();  iss.str(line);
					iss >> item;
					if (item == "#repeat") {
						iss >> item;  GLint rptTimes = stoi(item);
						GLint srcIndex = frameNum - 1;
						for (GLint i = 1; i < rptTimes; i++) {
							newAnim->cloneFrame(srcIndex, srcIndex + i);
						}
						frameNum += (rptTimes - 1);
						getline(fs, line);
						continue;
					}
					GLint rid = stoi(item);
					iss >> item;  GLint offset = stoi(item);
					iss >> item;  GLint cells = stoi(item);
					newAnim->setFrameImage(frameNum, rid, offset, cells);
					iss >> item;  GLint centerx = stoi(item);
					iss >> item;  GLint centery = stoi(item);
					newAnim->setFrameCenter(frameNum, centerx, centery);
					iss >> item;  endTime += stoi(item);
					newAnim->setEndTime(frameNum, endTime);
					GLint shiftx = 0, shifty = 0, dvx = 0, dvy = 0;
					iss >> item;
					while (item != "end:") {
						if (item == "shiftx:") {
							iss >> item;  shiftx = stoi(item);
						}
						else if (item == "shifty:") {
							iss >> item;  shifty = stoi(item);
						}
						else if (item == "dvx:") {
							iss >> item;  dvx = stoi(item);
						}
						else if (item == "dvy:") {
							iss >> item;  dvy = stoi(item);
						}
						// else if ..
						iss >> item;
					}
					newAnim->setShift(frameNum, shiftx, shifty);
					newAnim->setDv(frameNum, dvx, dvy);
					getline(fs, line);
					while (line.at(0) == '$') {
						newAnim->getFrame(frameNum)->addOptionalByStrAt(line);
						getline(fs, line);
					}
					frameNum++;
				}
				if (frameNum != newAnim->getFrameCount()) {
					// Error
				}
				obj->addAnimToSlot(animIndex, newAnim);
			}
			break;
		default:
			break;
		}
	}
	fs.close();
}

GLvoid AEFileLoader::loadObjectTable(string dataFileName) {
	ifstream fs(dataFileName);
	if (fs.fail()) {
		cout << "Failed: Cannot open \"objects.txt\"." << endl;
		return;
	}
	string line, item;
	istringstream iss;
	cout << "*** LOADING OBJECTS .. ***\n" << endl;
	while (!fs.eof()) {
		getline(fs, line);  iss.clear();  iss.str(line);
		if (line == "")
			continue;
		AEObject* obj = new AEObject();
		iss >> item;  GLint oid = stoi(item);  obj->setOid(oid);
		iss >> item;
		cout << "Object "<< item << " .. ";
		loadObject(obj, item);
		cout << "Done" << endl;
		oTable.addAt(oid, obj);
	}
	cout << "*** END LOADING OBJECTS ***\n" << endl;
	fs.close();
}