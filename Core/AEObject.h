/**
 * @file	AEObject.h
 * @brief	Defining the basic Objects used in our games.
 *
 * @author	CaiyZ (Mr.IroN)
 * @version	0.1
 * @date	2013
*/

#pragma once

#include <string>
#include "AEKeyboard.h"

using namespace std;


// Sparks
#define OBJECT_SPARK			99
#define SPARK_BLOCK				0
#define SPARK_HIT				1
#define SPARK_SLASH_LIGHT		2
#define SPARK_SLASH_HEAVY		3
#define SPARK_TINY				4
#define SPARK_SPECIAL			5
#define BLOOD_1					6
#define BLOOD_CRITICAL			7

// Types of Objects.
typedef enum _ObjType {
	OBJ_CHARACTER, OBJ_PROJECTILE
} ObjType;

// "$attack" parameter group 
typedef struct _AtkParas {
	GLint type, effect;
	GLint blockedTo, blownOffTo;
	GLint damage, force;
	GLint x1, y1, x2, y2;
	GLint angle;
} AtkParas;

// "$body" parameter group
typedef struct _BdyParas {
	GLint type;
	GLint x1, y1, x2, y2;
} BdyParas;

// "$block" parameter group
typedef struct _BlockParas {
	GLint type;
	GLint blockTo, breakTo;
} BlockParas;

// "$cast" parameter group
typedef struct _CastParas {
	GLint oid, action;
	GLint x, y;
} CastParas;

// "$hold" parameter group
typedef struct _HoldParas {
	GLbyte key;
} HoldParas;

// "$jumpto" parameter group
typedef struct _jumpParas {
	GLint action[AEKeyboardHandler::INPUT_COUNT];
} JumpParas;

// "$keyrelease" parameter group
typedef struct _KeyReleaseParas {
	GLbyte key;
	GLint action;
} KeyReleaseParas;


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
	HoldParas* hold;
	JumpParas* jumpTo;
	AtkParas* attack;
	BdyParas* body;
	BlockParas* block;
	CastParas* cast;
	KeyReleaseParas* keyRelease;
} Frame;

// Animation Class: Arranges what and how frames should be displayed.
class AEAnimation {

public:

	AEAnimation();
	GLvoid setFrameImage(GLint index, GLint _rid, GLint _offset, GLint _cells);
	GLvoid setFrameCenter(GLint index, GLint _centerx, GLint _centery);
	GLvoid cloneFrame(GLint srcIndex, GLint dstIndex);
	GLvoid init(GLint _frameCount, GLint _isLoop);
	GLvoid setName(string _name) { name == _name; }
	GLvoid setState(GLint _state) { state = _state; }
	GLvoid setNext(GLint _next) { next = _next; }
	GLvoid setTTL(GLint _timeToLive) { timeToLive = _timeToLive; }
	GLvoid setEndTime(GLint index, GLint _endTime) { endTimes[index] = _endTime; }
	GLvoid setShiftx(GLint index, GLint _shiftx) { frames[index].shiftx = _shiftx; }
	GLvoid setShifty(GLint index, GLint _shifty) { frames[index].shifty = _shifty; }
	GLvoid setDvx(GLint index, GLint _dvx) { frames[index].dvx = _dvx; }
	GLvoid setDvy(GLint index, GLint _dvy) { frames[index].dvy = _dvy; }
	GLvoid setRotate(GLint index, GLint _rotate) { frames[index].rotate = _rotate; }
	GLvoid setCast(GLint index, GLint _oid, GLint _action, GLint _castx, GLint _casty);
	GLvoid setAttack(GLint index, GLint _type, GLint _blockedTo, GLint _blownOffTo, GLint _damage, GLint _force, GLint _x1, GLint _y1, GLint _x2, GLint _y2, GLint _effect, GLint _angle);
	GLvoid setBody(GLint index, GLint _type, GLint _x1, GLint _y1, GLint _x2, GLint _y2);
	GLvoid setBlock(GLint index, GLint _type, GLint _blockTo, GLint _breakTo);
	GLvoid setHoldKey(GLint index, GLbyte _key);
	GLvoid setJump(GLint index, GLint _action, GLbyte _input);
	GLvoid setKeyRelease(GLint index, GLbyte _key, GLint _action);
	Frame getFrame(GLint index) { return frames[index]; }
	GLint getEndTime(GLint index) { return endTimes[index]; }
	GLint getFrameCount() { return frameCount; }
	GLint isLoop() { return animLoop; }
	GLint getNext() { return next; }
	GLint getTTL() { return timeToLive; }
	GLint getState() { return state; }

private:

	// Defining frames. Each frame has a time value to indicate that when it would change to next frame.
	GLint frameCount;
	Frame* frames;
	GLint* endTimes;

	// Indicating whether this animation is loopable.
	// If so, then we should define its time to live. When it is expired, it would change to its next animation.
	// If not so, this animation would change to next animation just when it finishes.
	GLint animLoop, timeToLive, next;
	
	// When displaying this Animation, the object is at which state.
	GLint state;

	// Animation name. May be used.
	string name;

};

// OBJECT: a complex union consists of various kinds of data:
// - OID: Every object holds an unique ID, so they could be easily referred
//   by the Sprites. OID is just the index number they occupies in the Object
//   Table.
// - ANIMATION: An object holds several animation arrays, each of them could
//   be called as "actions". These actions are defined in the certain object's
//   "data.txt". An active Sprite on the screen can freely change its action
//   from one to another. This happens when this Sprite moves, jumps, takes
//   damage, etc.
// - OTYPE: Different kinds of object must be differently dealed with. e.g.
//   when hitting a guy, an energy ball would explode, but a character would
//   not.
class AEObject {

public:

	static const GLint MAX_OBJECT_ACTIONS		= 100;

	static const GLint STATE_CHAR_STAND			= 0;
	static const GLint STATE_CHAR_FORWARD		= 1;
	static const GLint STATE_CHAR_BACKWARD		= 2;
	static const GLint STATE_CHAR_ACTION		= 3;
	static const GLint STATE_CHAR_ATK_H			= 5;
	static const GLint STATE_CHAR_ATK_V			= 6;
	static const GLint STATE_CHAR_ATK			= 7;
	static const GLint STATE_CHAR_DEAD			= 99;
	static const GLint STATE_CHAR_INAIR			= 100;
	static const GLint STATE_CHAR_FALLING		= 101;
	static const GLint STATE_CHAR_FALLING_R		= 102;
	static const GLint STATE_PROJ_FLY			= 1000;
	static const GLint STATE_PROJ_HIT			= 1001;
	static const GLint STATE_PROJ_DESTROYED		= 1002;

	static const GLint ACT_CHAR_STAND			= 0;
	static const GLint ACT_CHAR_FORWARD			= 1;
	static const GLint ACT_CHAR_BACKWARD		= 2;
	static const GLint ACT_CHAR_ATTACK_H		= 20;
	static const GLint ACT_CHAR_ATTACK_V		= 21;
	static const GLint ACT_CHAR_ATTACK			= 22;
	static const GLint ACT_CHAR_GET_HIT			= 30;
	static const GLint ACT_CHAR_DEFEND_H		= 4;
	static const GLint ACT_CHAR_DEFEND_V		= 7;
	static const GLint ACT_CHAR_DEAD			= 90;
	static const GLint ACT_CHAR_DEAD_NORM		= 91;
	static const GLint ACT_CHAR_LYING			= 92;
	static const GLint ACT_CHAR_INAIR			= 40;
	static const GLint ACT_CHAR_LANDING			= 45;
	static const GLint ACT_CHAR_FALL_90			= 80;
	static const GLint ACT_CHAR_FALL_60			= 81;
	static const GLint ACT_CHAR_FALL_30			= 82;
	static const GLint ACT_CHAR_FALL_0			= 83;
	static const GLint ACT_CHAR_FALL_DROP		= 84;
	static const GLint ACT_CHAR_FALL_REBOUND	= 85;
	static const GLint ACT_PROJ_FLY				= 0;
	static const GLint ACT_PROJ_HIT				= 10;
	static const GLint ACT_PROJ_DESTROYED		= 20;
	static const GLint ACT_PROJ_DISAPPEAR		= 30;

	static const GLint INPUT_COUNT				= 13;

	static GLbyte keyStrToByte(string keyStr);
	static GLbyte keyStrToInputCode(string keyStr);

	GLvoid addAnimToSlot(GLint slot, AEAnimation* _anim) { anim[slot] = _anim; }
	GLvoid setOid(GLint _oid) { oid = _oid; }
	GLvoid setName(string _name) { name = _name; }
	GLvoid setType(GLint _type) {
		switch (_type) {
		case 0: otype = OBJ_CHARACTER;  break;
		case 1: otype = OBJ_PROJECTILE;  break;
		default: break;
		}
	}
	AEAnimation* getAnim(GLint index) { return anim[index]; }
	ObjType getType() { return otype; }
	string getName() { return name; }

private:

	GLint oid;
	string name;
	AEAnimation* anim[MAX_OBJECT_ACTIONS];
	ObjType otype;

};


class AEObjectTable {

public:

	static const GLint MAX_TOTAL_OBJECTS		= 100;

	GLvoid addAt(GLint index, AEObject* obj) { table[index] = obj; }
	AEObject* get(GLint index) { return table[index]; }

private:

	AEObject* table[MAX_TOTAL_OBJECTS];

};