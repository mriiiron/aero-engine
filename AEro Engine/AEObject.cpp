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
#include <fstream>
#include <GL\glut.h>
#include "AESystemParam.h"
#include "AEUtility.h"
#include "AEResource.h"
#include "AEObject.h"

using namespace std;

extern AEResourceTable rTable;
extern AEObjectTable oTable;

AEAnimation::AEAnimation() {
	frames = NULL;  endTimes = NULL;
	frameCount = animLoop = next = state = 0;
	timeToLive = -1;
	name == string("Unnamed Animation");
}

GLvoid AEAnimation::init(GLint _frameCount, GLint _isLoop) {
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

GLvoid AEAnimation::setCast(GLint index, GLint _oid, GLint _action, GLint _castx, GLint _casty) {
	frames[index].cast = new CastParas;
	frames[index].cast->oid = _oid;
	frames[index].cast->action = _action;
	frames[index].cast->x = _castx;
	frames[index].cast->y = _casty;
}

GLvoid AEAnimation::setAttack(GLint index, GLint _type, GLint _blockedTo, GLint _blownOffTo, GLint _damage, GLint _force, GLint _x1, GLint _y1, GLint _x2, GLint _y2, GLint _effect, GLint _angle) {
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

GLvoid AEAnimation::setBody(GLint index, GLint _type, GLint _x1, GLint _y1, GLint _x2, GLint _y2) {
	frames[index].body = new BdyParas;
	frames[index].body->type = _type;
	frames[index].body->x1 = _x1;
	frames[index].body->y1 = _y1;
	frames[index].body->x2 = _x2;
	frames[index].body->y2 = _y2;
}

GLvoid AEAnimation::setBlock(GLint index, GLint _type, GLint _blockTo, GLint _breakTo) {
	frames[index].block = new BlockParas;
	frames[index].block->type = _type;
	frames[index].block->blockTo = _blockTo;
	frames[index].block->breakTo = _breakTo;
}

GLvoid AEAnimation::setHoldKey(GLint index, GLbyte _key) {
	frames[index].hold = new HoldParas;
	frames[index].hold->key = _key;
}

GLvoid AEAnimation::setJump(GLint index, GLint _action, GLbyte _input) {
	if (frames[index].jumpTo == NULL) {
		frames[index].jumpTo = new JumpParas;
		for (GLint i = 0; i < AEObject::INPUT_COUNT; i++) frames[index].jumpTo->action[i] = 0;
	}
	frames[index].jumpTo->action[_input] = _action;
}

GLvoid AEAnimation::setKeyRelease(GLint index, GLbyte _key, GLint _action) {
	frames[index].keyRelease = new KeyReleaseParas;
	frames[index].keyRelease->key = _key;
	frames[index].keyRelease->action = _action;
}

GLvoid AEAnimation::setFrameImage(GLint index, GLint _rid, GLint _offset, GLint _cells) {
	frames[index].rid = _rid;
	frames[index].imgOffset = _offset;
	frames[index].imgCells = _cells;
	frames[index].width = _cells * rTable.get(_rid)->getCellWidth();
	frames[index].height = rTable.get(_rid)->getCellHeight();
}

GLvoid AEAnimation::setFrameCenter(GLint index, GLint _centerx, GLint _centery) {
	frames[index].centerx = _centerx;
	frames[index].centery = _centery;
}

GLvoid AEAnimation::cloneFrame(GLint srcIndex, GLint dstIndex) {
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
		for (GLint i = 0; i < AEObject::INPUT_COUNT; i++) {
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

GLbyte AEObject::keyStrToByte(string keyStr) {
	if (keyStr == "none") {
		return AEKeyboardHandler::KS_NONE;
	}
	else if (keyStr == "attack") {
		return AEKeyboardHandler::BKS_ATTACK;
	}
	else if (keyStr == "jump") {
		return AEKeyboardHandler::BKS_JUMP;
	}
	else if (keyStr == "defend") {
		return AEKeyboardHandler::BKS_DEFEND;
	}
	else if (keyStr == "up") {
		return AEKeyboardHandler::AKS_UP;
	}
	else if (keyStr == "down") {
		return AEKeyboardHandler::AKS_DOWN;
	}
	else if (keyStr == "left") {
		return AEKeyboardHandler::AKS_LEFT;
	}
	else if (keyStr == "right") {
		return AEKeyboardHandler::AKS_RIGHT;
	}
	return 0;
}

GLbyte AEObject::keyStrToInputCode(string keyStr) {
	if (keyStr == "i_8") {
		return AEKeyboardHandler::INPUT_8;
	}
	else if (keyStr == "i_2") {
		return AEKeyboardHandler::INPUT_2;
	}
	else if (keyStr == "i_4") {
		return AEKeyboardHandler::INPUT_4;
	}
	else if (keyStr == "i_6") {
		return AEKeyboardHandler::INPUT_6;
	}
	else if (keyStr == "i_A") {
		return AEKeyboardHandler::INPUT_A;
	}
	else if (keyStr == "i_J") {
		return AEKeyboardHandler::INPUT_J;
	}
	else if (keyStr == "i_D") {
		return AEKeyboardHandler::INPUT_D;
	}
	else if (keyStr == "i_8A") {
		return AEKeyboardHandler::INPUT_8A;
	}
	else if (keyStr == "i_2A") {
		return AEKeyboardHandler::INPUT_2A;
	}
	else if (keyStr == "i_4A") {
		return AEKeyboardHandler::INPUT_4A;
	}
	else if (keyStr == "i_6A") {
		return AEKeyboardHandler::INPUT_6A;
	}
	else if (keyStr == "i_236A") {
		return AEKeyboardHandler::INPUT_236A;
	}
	else if (keyStr == "i_8D") {
		return AEKeyboardHandler::INPUT_8D;
	}
	return 0;
}