#define _CRT_SECURE_NO_WARNINGS

/*******************************************************************
== File Info ==
Name:				objects.cpp
Implementions of:	objects.h
Author:				CaiyZ (Mr.IroN)

== File Comment ==
This file contains the implementions of "objects.h".
When fetching graphical data, it may refer to Resource Table.
Moreover the Fonts are also loaded while loading Objects.
*******************************************************************/

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <GL\glut.h>
#include "AEUtility.h"
#include "AEResource.h"
#include "AEKeyboard.h"
#include "AEObject.h"

extern AEResourceTable rTable;
extern AEObjectTable oTable;

extern char* context;

Animation::Animation() {
	frames = NULL;  endTimes = NULL;
	frameCount = animLoop = next = state = 0;
	timeToLive = -1;
	strcpy(name, "");
}

GLvoid Animation::init(GLint _frameCount, GLint _isLoop) {
	frameCount = _frameCount;  animLoop = _isLoop;
	frames = new Frame[_frameCount];
	endTimes = new GLint[_frameCount];
	for (int i = 0; i < _frameCount; i++) {
		setCast(i, -1, 0, 0, 0);
		frames[i].jumpTo = NULL;
		frames[i].hold = NULL;
		frames[i].block = NULL;
		frames[i].attack = NULL;
		frames[i].body = NULL;
		frames[i].cast = NULL;
		frames[i].keyRelease = NULL;
	}
}

GLvoid Animation::setCast(GLint index, GLint _oid, GLint _action, GLint _castx, GLint _casty) {
	frames[index].cast = new CastParas;
	frames[index].cast->oid = _oid;
	frames[index].cast->action = _action;
	frames[index].cast->x = _castx;
	frames[index].cast->y = _casty;
}

GLvoid Animation::setAttack(GLint index, GLint _type, GLint _blockedTo, GLint _blownOffTo, GLint _damage, GLint _force, GLint _x1, GLint _y1, GLint _x2, GLint _y2, GLint _effect, GLint _angle) {
	frames[index].attack = new AtkParas;
	frames[index].attack->type = _type;
	frames[index].attack->effect = _effect;
	frames[index].attack->blockedTo = _blockedTo;
	frames[index].attack->blownOffTo = _blownOffTo;
	frames[index].attack->damage = _damage;
	frames[index].attack->force = _force;
	frames[index].attack->x1 = _x1;
	frames[index].attack->y1 = _y1;
	frames[index].attack->x2 = _x2;
	frames[index].attack->y2 = _y2;
	frames[index].attack->angle = _angle;
}

GLvoid Animation::setBody(GLint index, GLint _type, GLint _x1, GLint _y1, GLint _x2, GLint _y2) {
	frames[index].body = new BdyParas;
	frames[index].body->type = _type;
	frames[index].body->x1 = _x1;
	frames[index].body->y1 = _y1;
	frames[index].body->x2 = _x2;
	frames[index].body->y2 = _y2;
}

GLvoid Animation::setBlock(GLint index, GLint _type, GLint _blockTo, GLint _breakTo) {
	frames[index].block = new BlockParas;
	frames[index].block->type = _type;
	frames[index].block->blockTo = _blockTo;
	frames[index].block->breakTo = _breakTo;
}

GLvoid Animation::setHoldKey(GLint index, GLbyte _key) {
	frames[index].hold = new HoldParas;
	frames[index].hold->key = _key;
}

GLvoid Animation::setJump(GLint index, GLint _action, GLbyte _input) {
	if (frames[index].jumpTo == NULL) {
		frames[index].jumpTo = new JumpParas;
		for (GLint i = 0; i < AEKeyboardHandler::INPUT_COUNT; i++) frames[index].jumpTo->action[i] = 0;
	}
	frames[index].jumpTo->action[_input] = _action;
}

GLvoid Animation::setKeyRelease(GLint index, GLbyte _key, GLint _action) {
	frames[index].keyRelease = new KeyReleaseParas;
	frames[index].keyRelease->key = _key;
	frames[index].keyRelease->action = _action;
}

GLvoid Animation::setFrameImage(GLint index, GLint _rid, GLint _offset, GLint _cells) {
	frames[index].rid = _rid;
	frames[index].imgOffset = _offset;
	frames[index].imgCells = _cells;
	frames[index].width = _cells * rTable.get(_rid).getCellWidth();
	frames[index].height = rTable.get(_rid).getCellHeight();
}

GLvoid Animation::setFrameCenter(GLint index, GLint _centerx, GLint _centery) {
	frames[index].centerx = _centerx;
	frames[index].centery = _centery;
}

GLvoid Animation::cloneFrame(GLint srcIndex, GLint dstIndex) {
	if (srcIndex < 0 || srcIndex >= frameCount || dstIndex < 0 || dstIndex >= frameCount) {
		// Error
	}
	frames[dstIndex].rid = frames[srcIndex].rid;
	frames[dstIndex].imgOffset = frames[srcIndex].imgOffset;  frames[dstIndex].imgCells = frames[srcIndex].imgCells;
	frames[dstIndex].width = frames[srcIndex].width;  frames[dstIndex].height = frames[srcIndex].height;
	frames[dstIndex].centerx = frames[srcIndex].centerx;  frames[dstIndex].centery = frames[srcIndex].centery;
	frames[dstIndex].shiftx = frames[srcIndex].shiftx;  frames[dstIndex].shifty = frames[srcIndex].shifty;
	frames[dstIndex].dvx = frames[srcIndex].dvx;  frames[dstIndex].dvy = frames[srcIndex].dvy;
	frames[dstIndex].rotate = frames[srcIndex].rotate;
	if (frames[srcIndex].hold != NULL) {
		setHoldKey(dstIndex, frames[srcIndex].hold->key);
	}
	if (frames[srcIndex].jumpTo != NULL) {
		frames[dstIndex].jumpTo = new JumpParas;
		for (GLint i = 0; i < AEKeyboardHandler::INPUT_COUNT; i++) {
			frames[dstIndex].jumpTo->action[i] = frames[srcIndex].jumpTo->action[i];
		}
	}
	if (frames[srcIndex].attack != NULL) {
		AtkParas* srcAtk = frames[srcIndex].attack;
		setAttack(dstIndex, srcAtk->type, srcAtk->blockedTo, srcAtk->blownOffTo, srcAtk->damage, srcAtk->force, srcAtk->x1, srcAtk->y1, srcAtk->x2, srcAtk->y2, srcAtk->effect, srcAtk->angle);
	}
	if (frames[srcIndex].body != NULL) {
		BdyParas* srcBdy = frames[srcIndex].body;
		setBody(dstIndex, srcBdy->type, srcBdy->x1, srcBdy->y1, srcBdy->x2, srcBdy->y2);
	}
	if (frames[srcIndex].block != NULL) {
		BlockParas* srcBlock = frames[srcIndex].block;
		setBlock(dstIndex, srcBlock->type, srcBlock->blockTo, srcBlock->breakTo);
	}
	if (frames[srcIndex].cast != NULL) {
		CastParas* srcCast = frames[srcIndex].cast;
		setCast(dstIndex, srcCast->oid, srcCast->action, srcCast->x, srcCast->y);
	}
	if (frames[srcIndex].keyRelease != NULL) {
		KeyReleaseParas* srcRelease = frames[srcIndex].keyRelease;
		setKeyRelease(dstIndex, srcRelease->key, srcRelease->action);
	}
}

GLbyte keyStrToByte(const char* keyStr) {
	if (strcmp(keyStr, "none") == 0) {
		return AEKeyboardHandler::KS_NONE;
	}
	else if (strcmp(keyStr, "attack") == 0) {
		return AEKeyboardHandler::BKS_ATTACK;
	}
	else if (strcmp(keyStr, "jump") == 0) {
		return AEKeyboardHandler::BKS_JUMP;
	}
	else if (strcmp(keyStr, "defend") == 0) {
		return AEKeyboardHandler::BKS_DEFEND;
	}
	else if (strcmp(keyStr, "up") == 0) {
		return AEKeyboardHandler::AKS_UP;
	}
	else if (strcmp(keyStr, "down") == 0) {
		return AEKeyboardHandler::AKS_DOWN;
	}
	else if (strcmp(keyStr, "left") == 0) {
		return AEKeyboardHandler::AKS_LEFT;
	}
	else if (strcmp(keyStr, "right") == 0) {
		return AEKeyboardHandler::AKS_RIGHT;
	}
	return 0;
}

GLbyte keyStrToInputCode(const char* keyStr) {
	if (strcmp(keyStr, "i_8") == 0) {
		return AEKeyboardHandler::INPUT_8;
	}
	else if (strcmp(keyStr, "i_2") == 0) {
		return AEKeyboardHandler::INPUT_2;
	}
	else if (strcmp(keyStr, "i_4") == 0) {
		return AEKeyboardHandler::INPUT_4;
	}
	else if (strcmp(keyStr, "i_6") == 0) {
		return AEKeyboardHandler::INPUT_6;
	}
	else if (strcmp(keyStr, "i_A") == 0) {
		return AEKeyboardHandler::INPUT_A;
	}
	else if (strcmp(keyStr, "i_J") == 0) {
		return AEKeyboardHandler::INPUT_J;
	}
	else if (strcmp(keyStr, "i_D") == 0) {
		return AEKeyboardHandler::INPUT_D;
	}
	else if (strcmp(keyStr, "i_8A") == 0) {
		return AEKeyboardHandler::INPUT_8A;
	}
	else if (strcmp(keyStr, "i_2A") == 0) {
		return AEKeyboardHandler::INPUT_2A;
	}
	else if (strcmp(keyStr, "i_4A") == 0) {
		return AEKeyboardHandler::INPUT_4A;
	}
	else if (strcmp(keyStr, "i_6A") == 0) {
		return AEKeyboardHandler::INPUT_6A;
	}
	else if (strcmp(keyStr, "i_236A") == 0) {
		return AEKeyboardHandler::INPUT_236A;
	}
	else if (strcmp(keyStr, "i_8D") == 0) {
		return AEKeyboardHandler::INPUT_8D;
	}
	return 0;
}

GLvoid AEObject::loadDataByObjName(char* objectName) {
	char path[100];
	char line[MAX_CHAR_COUNT_IN_LINE];
	char* item;
	sprintf(path, "res\\%s\\data.txt", objectName);
	std::ifstream fs(path);
	while (!fs.eof()) {
	fs.getline(line, MAX_CHAR_COUNT_IN_LINE);
		switch (line[0]) {
		case '%':
			break;
		case '#':
			item = strtok_s(line, " ", &context);
			if (strcmp(item, "#name") == 0) {
				item = strtok_s(NULL, " ", &context);
				setName(item);
			}
			else if (strcmp(item, "#type") == 0) {
				item = strtok_s(NULL, " ", &context);
				setType(atoi(item));
			}
			else if (strcmp(item, "#action") == 0) {
				Animation newAnim;
				item = strtok_s(NULL, " ", &context);
				GLint animIndex = atoi(item);
				item = strtok_s(NULL, " ", &context);
				newAnim.setName(item);
				item = strtok_s(NULL, " ", &context);
				newAnim.setState(atoi(item));
				fs.getline(line, sizeof(line));
				item = strtok_s(line, " ", &context);
				if (strcmp(item, "#framecount") == 0) {
					item = strtok_s(NULL, " ", &context);
					GLint count = atoi(item);
					item = strtok_s(NULL, " ", &context);
					GLint loop = 0;
					if (strcmp(item, "loop") == 0) {		
						loop = 1;
						item = strtok_s(NULL, " ", &context);
						GLint ttl = atoi(item);
						newAnim.setTTL(ttl);
						if (ttl != -1) {
							fs.getline(line, sizeof(line));
							item = strtok_s(line, " ", &context);
							if (strcmp(item, "#loopover") == 0) {
								item = strtok_s(NULL, " ", &context);
								newAnim.setNext(atoi(item));
							}
							else {
								// Error
							}
						}
						else {
							newAnim.setNext(-1);
						}
					}
					else if (strcmp(item, "next") == 0) {
						loop = 0;
						item = strtok_s(NULL, " ", &context);
						newAnim.setNext(atoi(item));
					}
					else {
						// Error
					}
					newAnim.init(count, loop);
				}
				else {
					// Error
				}
				fs.getline(line, MAX_CHAR_COUNT_IN_LINE);
				GLint frameNum = 0, endTime = 0;
				while (strcmp(line, "#endaction") != 0) {
					item = strtok_s(line, " ", &context);
					if (strcmp(item, "#repeat") == 0) {
						item = strtok_s(NULL, " ", &context);  GLint rptTimes = atoi(item);
						GLint srcIndex = frameNum - 1;
						for (GLint i = 1; i < rptTimes; i++) {
							newAnim.cloneFrame(srcIndex, srcIndex + i);
						}
						frameNum += (rptTimes - 1);
						fs.getline(line, sizeof(line));
						continue;
					}
					GLint rid = atoi(item);
					item = strtok_s(NULL, " ", &context);  GLint offset = atoi(item);
					item = strtok_s(NULL, " ", &context);  GLint cells = atoi(item);
					newAnim.setFrameImage(frameNum, rid, offset, cells);
					item = strtok_s(NULL, " ", &context);  GLint centerx = atoi(item);
					item = strtok_s(NULL, " ", &context);  GLint centery = atoi(item);
					newAnim.setFrameCenter(frameNum, centerx, centery);
					item = strtok_s(NULL, " ", &context);  endTime += atoi(item);
					newAnim.setEndTime(frameNum, endTime);
					newAnim.setShiftx(frameNum, 0);  newAnim.setShifty(frameNum, 0);
					newAnim.setDvx(frameNum, 0);  newAnim.setDvy(frameNum, 0);
					newAnim.setRotate(frameNum, 0);
					item = strtok_s(NULL, " ", &context);
					while (strcmp(item, "end:") != 0) {
						if (strcmp(item, "shiftx:") == 0) {
							item = strtok_s(NULL, " ", &context);  newAnim.setShiftx(frameNum, atoi(item));
						}
						else if (strcmp(item, "shifty:") == 0) {
							item = strtok_s(NULL, " ", &context);  newAnim.setShifty(frameNum, atoi(item));
						}
						else if (strcmp(item, "dvx:") == 0) {
							item = strtok_s(NULL, " ", &context);  newAnim.setDvx(frameNum, atoi(item));
						}
						else if (strcmp(item, "dvy:") == 0) {
							item = strtok_s(NULL, " ", &context);  newAnim.setDvy(frameNum, atoi(item));
						}
						else if (strcmp(item, "rotate:") == 0) {
							item = strtok_s(NULL, " ", &context);  newAnim.setRotate(frameNum, atoi(item));
						}
						// else if ..
						item = strtok_s(NULL, " ", &context);
					}
					fs.getline(line, sizeof(line));
					while (line[0] == '$') {
						item = strtok_s(line, " ", &context);
						if (strcmp(item, "$cast") == 0) {
							GLint oid, action, castx, casty;
							item = strtok_s(NULL, " ", &context);  oid = atoi(item);
							item = strtok_s(NULL, " ", &context);  action = atoi(item);
							item = strtok_s(NULL, " ", &context);  castx = atoi(item);
							item = strtok_s(NULL, " ", &context);  casty = atoi(item);
							newAnim.setCast(frameNum, oid, action, castx, casty);
						}
						else if (strcmp(item, "$attack") == 0) {
							GLint atkType, blockedTo, blownOffTo, damage, x1, y1, x2, y2, effect = 0, angle = 0, force = 0;
							item = strtok_s(NULL, " ", &context);  atkType = atoi(item);
							item = strtok_s(NULL, " ", &context);  blockedTo = atoi(item);
							item = strtok_s(NULL, " ", &context);  blownOffTo = atoi(item);
							item = strtok_s(NULL, " ", &context);  damage = atoi(item);
							item = strtok_s(NULL, " ", &context);  x1 = atoi(item);
							item = strtok_s(NULL, " ", &context);  y1 = atoi(item);
							item = strtok_s(NULL, " ", &context);  x2 = atoi(item);
							item = strtok_s(NULL, " ", &context);  y2 = atoi(item);
							item = strtok_s(NULL, " ", &context);
							while (strcmp(item, "end:") != 0) {
								if (strcmp(item, "effect:") == 0) {
									item = strtok_s(NULL, " ", &context);  effect = atoi(item);
								}
								else if (strcmp(item, "angle:") == 0) {
									item = strtok_s(NULL, " ", &context);  angle = atoi(item);
								}
								else if (strcmp(item, "force:") == 0) {
									item = strtok_s(NULL, " ", &context);  force = atoi(item);
								}
								// else if ..
								item = strtok_s(NULL, " ", &context);
							}
							newAnim.setAttack(frameNum, atkType, blockedTo, blownOffTo, damage, force, x1, y1, x2, y2, effect, angle);
						}
						else if (strcmp(item, "$body") == 0) {
							GLint bdyType, x1, y1, x2, y2;
							item = strtok_s(NULL, " ", &context);  bdyType = atoi(item);
							item = strtok_s(NULL, " ", &context);  x1 = atoi(item);
							item = strtok_s(NULL, " ", &context);  y1 = atoi(item);
							item = strtok_s(NULL, " ", &context);  x2 = atoi(item);
							item = strtok_s(NULL, " ", &context);  y2 = atoi(item);
							newAnim.setBody(frameNum, bdyType, x1, y1, x2, y2);
						}
						else if (strcmp(item, "$block") == 0) {
							GLint blockType, blockTo, breakTo;
							item = strtok_s(NULL, " ", &context);  blockType = atoi(item);
							item = strtok_s(NULL, " ", &context);  blockTo = atoi(item);
							item = strtok_s(NULL, " ", &context);  breakTo = atoi(item);
							newAnim.setBlock(frameNum, blockType, blockTo, breakTo);
						}
						else if (strcmp(item, "$hold") == 0) {
							item = strtok_s(NULL, " ", &context);
							newAnim.setHoldKey(frameNum, keyStrToByte(item));
						}
						else if (strcmp(item, "$jumpto") == 0) {
							GLbyte input;  GLint action;
							item = strtok_s(NULL, " ", &context);  action = atoi(item);
							item = strtok_s(NULL, " ", &context);  input = keyStrToInputCode(item);
							newAnim.setJump(frameNum, action, input);
						}
						else if (strcmp(item, "$keyrelease") == 0) {
							GLbyte key;  GLint action;
							item = strtok_s(NULL, " ", &context);  action = atoi(item);
							item = strtok_s(NULL, " ", &context);
							key = keyStrToByte(item);
							newAnim.setKeyRelease(frameNum, key, action);
						}
						// Else if ..
						fs.getline(line, sizeof(line));
					}
					frameNum++;
				}
				if (frameNum != newAnim.getFrameCount()) {
					// Error
				}
				addAnimToSlot(animIndex, newAnim);
			}
			break;
		default:
			break;
		}
	}
	fs.close();
}

GLvoid AEObjectTable::loadObjectsFromFile(char* dataFileName) {
	std::ifstream fs(dataFileName);
	char line[MAX_CHAR_COUNT_IN_LINE];
	printf("*** LOADING OBJECTS .. ***\n");
	while (!fs.eof()) {
		fs.getline(line, sizeof(line));
		AEObject* obj = new AEObject();
		char* item = strtok_s(line, " ", &context);
		GLint oid = atoi(item);
		obj->setOid(oid);
		item = strtok_s(NULL, " ", &context);
		printf("%s .. ", item);
		obj->loadDataByObjName(item);
		printf("Done. \n");
		oTable.addAt(oid, obj);
	}
	printf("*** END LOADING OBJECTS ***\n\n");
	fs.close();
}