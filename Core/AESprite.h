/**
 * @file	AESprite.h
 * @brief	Defining all "dynamic objects" in the scenes.
 *
 * Sprites could be dynamically created and destroyed while the game is running.
 * A certain scene helds a sprite table to manage its sprites.
 *
 * @author	CaiyZ (Mr.IroN)
 * @version	0.1
 * @date	2013
*/

#pragma once

#include "AEKeyboard.h"
#include "AEObject.h"

using namespace std;


class AEScene;


class AESprite {

public:

	static const GLint TEAM_NONE				= 0;
	static const GLint TEAM_1					= 1;
	static const GLint TEAM_2					= 2;
	static const GLint TEAM_3					= 3;
	static const GLint TEAM_4					= 4;
	static const GLint CAST_INVERSE				= 1;
	static const GLint FACING_RIGHT				= 0;
	static const GLint FACING_LEFT				= 1;

	AESprite(AEScene* _scene, GLint _oid, GLint _team, GLfloat _cx, GLfloat _cy, GLint _action = 0, GLint inverse = 0);
	GLvoid setIndex(GLint _index) { index = _index; }
	GLvoid setState(GLint _state) { state = _state; }
	GLvoid setAx(GLfloat _ax) { ax = _ax; }
	GLvoid setAy(GLfloat _ay) { ay = _ay; }
	GLvoid setVx(GLfloat _vx) { vx = _vx; }
	GLvoid setVy(GLfloat _vy) { vy = _vy; }
	GLvoid setVAngle(GLfloat _vangle) { vangle = _vangle; }
	GLvoid setGroundSpeed(GLfloat _speed) { gndSpeed = _speed; }
	GLvoid setAngle(GLfloat _angle) { angle = _angle; }
	GLvoid rotateDeg(GLfloat degree) { angle += AEUtil::deg2rad(degree); }
	GLvoid rotateRad(GLfloat rad) { angle += rad; }
	GLvoid setFacing(GLint _facing) { facing = _facing; }
	GLvoid turnOver() { facing = 1 - facing; }
	GLvoid resetKeys() { keyState = 0; }
	GLvoid setHPValue(GLint _hpValue) { hpValue = _hpValue; }
	GLvoid stiffen(GLint _time) { timeToStiff = _time; }
	GLvoid lockAtkJudge() { atkJudgeLock = 1; }
	GLvoid unlockAtkJudge() { atkJudgeLock = 0; }
	GLvoid keyDown(GLint _key) { keyState = keyState | _key; }
	GLvoid keyUp(GLint _key) { keyState = keyState & ~_key; }
	GLbyte getFacing() { return facing; }
	GLbyte isAtkJudgeLocked() { return atkJudgeLock; }
	GLbyte isKeyDown(GLint _key) { return _key & keyState; }
	GLint getIndex() { return index; }
	GLint getOid() { return oid; }
	GLint getAction() { return action; }
	GLint getFrame() { return frame; }
	GLint getState() { return state; }
	GLint getTeam() { return team; }
	GLint getStiffTime() { return timeToStiff; }
	GLint getGroundSpeed() { return gndSpeed; }
	GLfloat getCx() { return cx; }
	GLfloat getCy() { return cy; }
	AEPoint getCenter() { return AEUtil::createPoint(cx, cy); }
	GLfloat getVx() { return vx; }
	GLfloat getVy() { return vy; }
	GLfloat getAngle() { return angle; }
	AEVector2 getFaceVector();
	GLbyte inputStateJudge(GLint _input);
	GLint log2(GLint key);
	AEPoint calcRotatedPoint(AEPoint point, GLfloat cx, GLfloat cy, AEFrame* f, GLfloat angle, GLbyte facing);
	AEBiasRect calcRotatedRect(GLfloat cx, GLfloat cy, AEFrame* f, GLfloat angle, GLbyte facing);
	/* CHARACTER ONLY */
	GLint getKeyState() { return keyState; }
	GLint getHP() { return hpValue; }
	AEScene* getScene() { return scene; }
	GLvoid takeDamage(GLint damage) { hpValue -= damage; }
	GLvoid pressUp();
	GLvoid pressDown();
	GLvoid pressLeft();
	GLvoid pressRight();
	GLvoid pressAttack();
	GLvoid pressJump();
	GLvoid pressDefend();
	GLvoid pressForward();
	GLvoid pressBackward();
	GLvoid releaseUp();
	GLvoid releaseDown();
	GLvoid releaseLeft();
	GLvoid releaseRight();
	GLvoid releaseAttack();
	GLvoid releaseJump();
	GLvoid releaseDefend();
	GLvoid testKeyState();
	/* END CHARACTER ONLY */
	/* AI ONLY */
	GLvoid input(GLint _input);
	/* END AI ONLY */
	string getObjName();
	GLvoid changeAction(GLint _action);
	GLvoid toNextFrame(AEAnimation anim);
	GLvoid paintShadow();
	GLvoid paintCrosshair();
	GLvoid paint();

	virtual GLvoid update();

protected:

	GLint index, oid, action, frame, time, timeToLive, timeToStiff;
	GLfloat cx, cy, vx, vy, ax, ay, angle, vangle, gndSpeed;
	GLint state, team, keyState, drop, onLandform;
	GLbyte facing, atkJudgeLock;
	GLint hpValue, hpMax;
	GLbyte inputState[AEKeyboardHandler::INPUT_COUNT];
	AEScene* scene;

};


class AESpriteTable {

public:

	static const GLint MAX_ONLINE_SPRITES		= 120;

	AESpriteTable();
	AESprite* get(GLint index) { return table[index]; }
	AESprite* getByHash(GLint hashIndex) { return table[hash[hashIndex]]; }
	GLint getHashCount() { return pHash; }
	GLvoid add(AESprite* sp);
	GLvoid addAt(GLint index, AESprite* sp);
	GLvoid remove(GLint index);
	GLvoid clear();
	GLvoid handleCollisions();
	GLvoid update();
	GLvoid paint();

private:

	AESprite* table[MAX_ONLINE_SPRITES];
	GLint occupied[MAX_ONLINE_SPRITES];
	GLint hash[MAX_ONLINE_SPRITES];
	GLint maxIndex, pHash;

};