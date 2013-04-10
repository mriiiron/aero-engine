#include <GL\glut.h>
#include <string>
#include "AEUtility.h"
#include "AEKeyboard.h"
#include "AEObjAnimation.h"

extern AEResourceTable rTable;

AEAnimation::AEAnimation(GLint _frameCount) {
	frameCount = _frameCount;
	endTime = new GLint[frameCount];
	for (GLint i = 0; i < frameCount; i++) {
		frame[i] = new AEFrame();
		endTime[i] = 0;
	}
	isAnimLoop = next = state = 0;
	timeToLive = -1;
}

//GLvoid AEAnimation::setCast(GLint index, GLint _oid, GLint _action, GLint _castx, GLint _casty) {
//	frames[index].cast = new CastParas;
//	frames[index].cast->oid = _oid;
//	frames[index].cast->action = _action;
//	frames[index].cast->x = _castx;
//	frames[index].cast->y = _casty;
//}
//
//GLvoid AEAnimation::setAttack(GLint index, GLint _type, GLint _blockedTo, GLint _blownOffTo, GLint _damage, GLint _force, GLint _x1, GLint _y1, GLint _x2, GLint _y2, GLint _effect, GLint _angle) {
//	frames[index].attack = new AtkParas;
//	frames[index].attack->type = _type;
//	frames[index].attack->effect = _effect;
//	frames[index].attack->blockedTo = _blockedTo;
//	frames[index].attack->blownOffTo = _blownOffTo;
//	frames[index].attack->damage = _damage;
//	frames[index].attack->force = _force;
//	frames[index].attack->x1 = _x1;
//	frames[index].attack->y1 = _y1;
//	frames[index].attack->x2 = _x2;
//	frames[index].attack->y2 = _y2;
//	frames[index].attack->angle = _angle;
//}
//
//GLvoid AEAnimation::setBody(GLint index, GLint _type, GLint _x1, GLint _y1, GLint _x2, GLint _y2) {
//	frames[index].body = new BdyParas;
//	frames[index].body->type = _type;
//	frames[index].body->x1 = _x1;
//	frames[index].body->y1 = _y1;
//	frames[index].body->x2 = _x2;
//	frames[index].body->y2 = _y2;
//}
//
//GLvoid AEAnimation::setBlock(GLint index, GLint _type, GLint _blockTo, GLint _breakTo) {
//	frames[index].block = new BlockParas;
//	frames[index].block->type = _type;
//	frames[index].block->blockTo = _blockTo;
//	frames[index].block->breakTo = _breakTo;
//}
//
//GLvoid AEAnimation::setHoldKey(GLint index, GLbyte _key) {
//	frames[index].hold = new HoldParas;
//	frames[index].hold->key = _key;
//}
//
//GLvoid AEAnimation::setJump(GLint index, GLint _action, GLbyte _input) {
//	if (frames[index].jumpTo == NULL) {
//		frames[index].jumpTo = new JumpParas;
//		for (GLint i = 0; i < AEObject::INPUT_COUNT; i++) frames[index].jumpTo->action[i] = 0;
//	}
//	frames[index].jumpTo->action[_input] = _action;
//}
//
//GLvoid AEAnimation::setKeyRelease(GLint index, GLbyte _key, GLint _action) {
//	frames[index].keyRelease = new KeyReleaseParas;
//	frames[index].keyRelease->key = _key;
//	frames[index].keyRelease->action = _action;
//}

GLvoid AEAnimation::setFrameImage(GLint index, GLint _rid, GLint _offset, GLint _cells) {
	AEResource* res = rTable.get(_rid);
	frame[index]->setImageFromRes(res, _offset, _cells);
	frame[index]->setSize(_cells * res->getCellWidth(), res->getCellHeight());
}

GLvoid AEAnimation::cloneFrame(GLint srcIndex, GLint dstIndex) {
	if (srcIndex < 0 || srcIndex >= frameCount || dstIndex < 0 || dstIndex >= frameCount) {
		// Error
	}
	frame[dstIndex]->setImageFromRes(frame[srcIndex]->getResource(), frame[srcIndex]->getImgOffset(), frame[srcIndex]->getImgCells());
	frame[dstIndex]->setSize(frame[srcIndex]->getWidth(), frame[srcIndex]->getHeight());
	frame[dstIndex]->setCenter(frame[srcIndex]->getCenterx(), frame[srcIndex]->getCentery());
	frame[dstIndex]->setShift(frame[srcIndex]->getShiftx(), frame[srcIndex]->getShifty());
	frame[dstIndex]->setDv(frame[srcIndex]->getDvx(), frame[srcIndex]->getDvy());
	// TODO: Copy Optionals

	//if (frames[srcIndex].hold != NULL) {
	//	setHoldKey(dstIndex, frames[srcIndex].hold->key);
	//}
	//if (frames[srcIndex].jumpTo != NULL) {
	//	frames[dstIndex].jumpTo = new JumpParas;
	//	for (GLint i = 0; i < AEObject::INPUT_COUNT; i++) {
	//		frames[dstIndex].jumpTo->action[i] = frames[srcIndex].jumpTo->action[i];
	//	}
	//}
	//if (frames[srcIndex].attack != NULL) {
	//	AtkParas* srcAtk = frames[srcIndex].attack;
	//	setAttack(dstIndex, srcAtk->type, srcAtk->blockedTo, srcAtk->blownOffTo, srcAtk->damage, srcAtk->force, srcAtk->x1, srcAtk->y1, srcAtk->x2, srcAtk->y2, srcAtk->effect, srcAtk->angle);
	//}
	//if (frames[srcIndex].body != NULL) {
	//	BdyParas* srcBdy = frames[srcIndex].body;
	//	setBody(dstIndex, srcBdy->type, srcBdy->x1, srcBdy->y1, srcBdy->x2, srcBdy->y2);
	//}
	//if (frames[srcIndex].block != NULL) {
	//	BlockParas* srcBlock = frames[srcIndex].block;
	//	setBlock(dstIndex, srcBlock->type, srcBlock->blockTo, srcBlock->breakTo);
	//}
	//if (frames[srcIndex].cast != NULL) {
	//	CastParas* srcCast = frames[srcIndex].cast;
	//	setCast(dstIndex, srcCast->oid, srcCast->action, srcCast->x, srcCast->y);
	//}
	//if (frames[srcIndex].keyRelease != NULL) {
	//	KeyReleaseParas* srcRelease = frames[srcIndex].keyRelease;
	//	setKeyRelease(dstIndex, srcRelease->key, srcRelease->action);
	//}
}