#pragma once

using namespace std;

#include "AEResource.h"


class AEFrameOptional {

public:

	virtual ~AEFrameOptional() {}

};


// Samples of Frame Optional

//// "$attack" parameter group 
//typedef struct _AtkParas {
//	GLint type, effect;
//	GLint blockedTo, blownOffTo;
//	GLint damage, force;
//	GLint x1, y1, x2, y2;
//	GLint angle;
//} AtkParas;
//
//// "$body" parameter group
//typedef struct _BdyParas {
//	GLint type;
//	GLint x1, y1, x2, y2;
//} BdyParas;
//
//// "$block" parameter group
//typedef struct _BlockParas {
//	GLint type;
//	GLint blockTo, breakTo;
//} BlockParas;
//
//// "$cast" parameter group
//typedef struct _CastParas {
//	GLint oid, action;
//	GLint x, y;
//} CastParas;
//
//// "$hold" parameter group
//typedef struct _HoldParas {
//	GLbyte key;
//} HoldParas;
//
//// "$jumpto" parameter group
//typedef struct _jumpParas {
//	GLint action[AEKeyboardHandler::INPUT_COUNT];
//} JumpParas;
//
//// "$keyrelease" parameter group
//typedef struct _KeyReleaseParas {
//	GLbyte key;
//	GLint action;
//} KeyReleaseParas;


class AEFrame {

public:

	static const GLint MAX_OPTIONALS			= 20;
	
	GLvoid setImageFromRes(AEResource* _res, GLint _imgOffset, GLint _imgCells) { res = _res;  imgOffset = _imgOffset;  imgCells = _imgCells; }
	GLvoid setSize(GLint _width, GLint _height) { width = _width;  height = _height; }
	GLvoid setCenter(GLint _centerx, GLint _centery) { centerx = _centerx;  centery = _centery; }
	GLvoid setShift(GLint _shiftx, GLint _shifty) { shiftx = _shiftx;  shifty = _shifty; }
	GLvoid setDv(GLint _dvx, GLint _dvy) { dvx = _dvx;  dvy = _dvy; }
	AEResource* getResource() { return res; }
	GLint getImgOffset() { return imgOffset; }
	GLint getImgCells() { return imgCells; }
	GLint getWidth() { return width; }
	GLint getHeight() { return height; }
	GLint getCenterx() { return centerx; }
	GLint getCentery() { return centery; }
	GLint getShiftx() { return shiftx; }
	GLint getShifty() { return shifty; }
	GLint getDvx() { return dvx; }
	GLint getDvy() { return dvy; }

	AEFrameOptional* getOptional(GLint index) { return optionalParam[index]; }
	GLvoid addOptionalByStrAt(string line);

private:

	AEResource* res;
	GLint imgOffset, imgCells;
	GLint width, height, centerx, centery;
	GLint shiftx, shifty, dvx, dvy;
	AEFrameOptional* optionalParam[MAX_OPTIONALS];

};


// FRAME: The component parts of Animation.
// - RID: The frame's image should be fetched from which Resource (see "resources.h").
// - IMGOFFSET, IMGCELLS: Image from which cell, and how many cells it occupies.
// - WIDTH, HEIGHT, CENTERX, CENTERY: Size and center point.
// - SHIFTX, SHIFTY, DVX, DVY, ROTATE: Moving controls.
typedef struct _Frame {
	GLint rid;
	GLint imgOffset, imgCells;
	GLint width, height, centerx, centery;
	GLint shiftx, shifty, dvx, dvy, rotate;
	//HoldParas* hold;
	//JumpParas* jumpTo;
	//AtkParas* attack;
	//BdyParas* body;
	//BlockParas* block;
	//CastParas* cast;
	//KeyReleaseParas* keyRelease;
} Frame;

// Animation Class: Arranges what and how frames should be displayed.
class AEAnimation {

public:

	static const GLint MAX_FRAMES				= 100;

	AEAnimation(GLint _frameCount);
	
	GLvoid cloneFrame(GLint srcIndex, GLint dstIndex);
	GLvoid setLoop(GLint _isLoop) { isAnimLoop = _isLoop; }
	GLvoid setState(GLint _state) { state = _state; }
	GLvoid setNext(GLint _next) { next = _next; }
	GLvoid setTTL(GLint _timeToLive) { timeToLive = _timeToLive; }
	GLvoid setEndTime(GLint index, GLint _endTime) { endTime[index] = _endTime; }
	GLvoid setFrameCenter(GLint index, GLint _centerx, GLint _centery) { frame[index]->setCenter(_centerx, _centery); }
	GLvoid setShift(GLint index, GLint _shiftx, GLint _shifty) { frame[index]->setShift(_shiftx, _shifty); }
	GLvoid setDv(GLint index, GLint _dvx, GLint _dvy) { frame[index]->setDv(_dvx, _dvy); }
	GLvoid setFrameImage(GLint index, GLint _rid, GLint _offset, GLint _cells);
	//GLvoid setRotate(GLint index, GLint _rotate) { frames[index].rotate = _rotate; }
	//GLvoid setCast(GLint index, GLint _oid, GLint _action, GLint _castx, GLint _casty);
	//GLvoid setAttack(GLint index, GLint _type, GLint _blockedTo, GLint _blownOffTo, GLint _damage, GLint _force, GLint _x1, GLint _y1, GLint _x2, GLint _y2, GLint _effect, GLint _angle);
	//GLvoid setBody(GLint index, GLint _type, GLint _x1, GLint _y1, GLint _x2, GLint _y2);
	//GLvoid setBlock(GLint index, GLint _type, GLint _blockTo, GLint _breakTo);
	//GLvoid setHoldKey(GLint index, GLbyte _key);
	//GLvoid setJump(GLint index, GLint _action, GLbyte _input);
	//GLvoid setKeyRelease(GLint index, GLbyte _key, GLint _action);
	AEFrame* getFrame(GLint index) { return frame[index]; }
	GLint getEndTime(GLint index) { return endTime[index]; }
	GLint getFrameCount() { return frameCount; }
	GLboolean isLoop() { return isAnimLoop; }
	GLint getNext() { return next; }
	GLint getTTL() { return timeToLive; }
	GLint getState() { return state; }

	virtual GLvoid loadFrameOptional(GLint index, GLint _slot, string str) {}

private:

	// Defining frames. Each frame has a time value to indicate that when it would change to next frame.
	GLint frameCount;
	AEFrame* frame[MAX_FRAMES];
	GLint* endTime;

	// Indicating whether this animation is loopable.
	// If so, then we should define its time to live. When it is expired, it would change to its next animation.
	// If not so, this animation would change to next animation just when it finishes.
	GLint timeToLive, next;
	
	GLboolean isAnimLoop;

	// When displaying this Animation, the object is at which state.
	GLint state;

};