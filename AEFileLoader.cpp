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

extern char* context;

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
				iss >> item;  res->setTexture(loadPNGTexture(item.c_str()));
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
				iss >> item;  font->setTexture(loadPNGTexture(item.c_str()));
				font->build();
				fontLib.add(font);
				getline(fs, line);  iss.clear();  iss.str(line);
			}
		}
		else if (line == "#bg") {
			getline(fs, line);  iss.str(line);
			while (line != "#bg_end") {
				AEBackground* bg = new AEBackground();
				iss >> item;  bg->loadFromFile(item.c_str());
				bgLib.add(bg);
				getline(fs, line);  iss.clear();  iss.str(line);
			}
		}
		else if (line == "#ptcl") {
			getline(fs, line);  iss.str(line);
			while (line != "#ptcl_end") {
				iss >> item;  GLint ptclTexIndex = stoi(item);
				iss >> item;  ptclSys.loadTexture(ptclTexIndex, item.c_str());
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
		getline(fs, line);  iss.clear();  iss.str(line);
		if (line == "") {
			continue;
		}
		switch (line.at(0)) {
		case '%':
			break;
		case '#':
			iss >> item;
			if (item == "#name") {
				iss >> item;  obj->setName(item);
			}
			else if (item == "#type") {
				iss >> item;  obj->setType(stoi(item));
			}
			else if (item == "#action") {
				Animation newAnim;
				iss >> item;  GLint animIndex = stoi(item);
				iss >> item;  newAnim.setName(item);
				iss >> item;  newAnim.setState(stoi(item));
				getline(fs, line);  iss.clear();  iss.str(line);
				iss >> item;
				if (item == "#framecount") {
					iss >> item;  GLint count = stoi(item);
					iss >> item;
					GLint loop = 0;
					if (item == "loop") {		
						loop = 1;
						iss >> item;  GLint ttl = stoi(item);
						newAnim.setTTL(ttl);
						if (ttl != -1) {
							getline(fs, line);  iss.clear();  iss.str(line);
							iss >> item;
							if (item == "#loopover") {
								iss >> item;  newAnim.setNext(stoi(item));
							}
							else {
								// Error
							}
						}
						else {
							newAnim.setNext(-1);
						}
					}
					else if (item == "next") {
						loop = 0;
						iss >> item;
						newAnim.setNext(stoi(item));
					}
					else {
						// Error
					}
					newAnim.init(count, loop);
				}
				else {
					// Error
				}
				getline(fs, line);  iss.clear();  iss.str(line);
				GLint frameNum = 0, endTime = 0;
				while (line != "#endaction") {
					iss >> item;
					if (item == "#repeat") {
						iss >> item;  GLint rptTimes = stoi(item);
						GLint srcIndex = frameNum - 1;
						for (GLint i = 1; i < rptTimes; i++) {
							newAnim.cloneFrame(srcIndex, srcIndex + i);
						}
						frameNum += (rptTimes - 1);
						getline(fs, line);  iss.clear();  iss.str(line);
						continue;
					}
					GLint rid = stoi(item);
					iss >> item;  GLint offset = stoi(item);
					iss >> item;  GLint cells = stoi(item);
					newAnim.setFrameImage(frameNum, rid, offset, cells);
					iss >> item;  GLint centerx = stoi(item);
					iss >> item;  GLint centery = stoi(item);
					newAnim.setFrameCenter(frameNum, centerx, centery);
					iss >> item;  endTime += stoi(item);
					newAnim.setEndTime(frameNum, endTime);
					newAnim.setShiftx(frameNum, 0);  newAnim.setShifty(frameNum, 0);
					newAnim.setDvx(frameNum, 0);  newAnim.setDvy(frameNum, 0);
					newAnim.setRotate(frameNum, 0);
					iss >> item;
					while (item != "end:") {
						if (item == "shiftx:") {
							iss >> item;  newAnim.setShiftx(frameNum, stoi(item));
						}
						else if (item == "shifty:") {
							iss >> item;  newAnim.setShifty(frameNum, stoi(item));
						}
						else if (item == "dvx:") {
							iss >> item;  newAnim.setDvx(frameNum, stoi(item));
						}
						else if (item == "dvy:") {
							iss >> item;  newAnim.setDvy(frameNum, stoi(item));
						}
						else if (item == "rotate:") {
							iss >> item;  newAnim.setRotate(frameNum, stoi(item));
						}
						// else if ..
						iss >> item;
					}
					getline(fs, line);  iss.clear();  iss.str(line);
					while (line.at(0) == '$') {
						iss >> item;
						if (item == "$cast") {
							GLint oid, action, castx, casty;
							iss >> item;  oid = stoi(item);
							iss >> item;  action = stoi(item);
							iss >> item;  castx = stoi(item);
							iss >> item;  casty = stoi(item);
							newAnim.setCast(frameNum, oid, action, castx, casty);
						}
						else if (item == "$attack") {
							GLint atkType, blockedTo, blownOffTo, damage, x1, y1, x2, y2, effect = 0, angle = 0, force = 0;
							iss >> item;  atkType = stoi(item);
							iss >> item;  blockedTo = stoi(item);
							iss >> item;  blownOffTo = stoi(item);
							iss >> item;  damage = stoi(item);
							iss >> item;  x1 = stoi(item);
							iss >> item;  y1 = stoi(item);
							iss >> item;  x2 = stoi(item);
							iss >> item;  y2 = stoi(item);
							iss >> item;
							while (item != "end:") {
								if (item == "effect:") {
									iss >> item;  effect = stoi(item);
								}
								else if (item == "angle:") {
									iss >> item;  angle = stoi(item);
								}
								else if (item == "force:") {
									iss >> item;  force = stoi(item);
								}
								// else if ..
								iss >> item;
							}
							newAnim.setAttack(frameNum, atkType, blockedTo, blownOffTo, damage, force, x1, y1, x2, y2, effect, angle);
						}
						else if (item == "$body") {
							GLint bdyType, x1, y1, x2, y2;
							iss >> item;  bdyType = stoi(item);
							iss >> item;  x1 = stoi(item);
							iss >> item;  y1 = stoi(item);
							iss >> item;  x2 = stoi(item);
							iss >> item;  y2 = stoi(item);
							newAnim.setBody(frameNum, bdyType, x1, y1, x2, y2);
						}
						else if (item == "$block") {
							GLint blockType, blockTo, breakTo;
							iss >> item;  blockType = stoi(item);
							iss >> item;  blockTo = stoi(item);
							iss >> item;  breakTo = stoi(item);
							newAnim.setBlock(frameNum, blockType, blockTo, breakTo);
						}
						else if (item == "$hold") {
							iss >> item;
							newAnim.setHoldKey(frameNum, AEObject::keyStrToByte(item));
						}
						else if (item == "$jumpto") {
							GLbyte input;  GLint action;
							iss >> item;  action = stoi(item);
							iss >> item;  input = AEObject::keyStrToInputCode(item);
							newAnim.setJump(frameNum, action, input);
						}
						else if (item == "$keyrelease") {
							GLbyte key;  GLint action;
							iss >> item;  action = stoi(item);
							iss >> item;
							key = AEObject::keyStrToByte(item);
							newAnim.setKeyRelease(frameNum, key, action);
						}
						// Else if ..
						getline(fs, line);  iss.clear();  iss.str(line);
					}
					frameNum++;
				}
				if (frameNum != newAnim.getFrameCount()) {
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