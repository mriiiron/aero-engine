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
#include "AEObjAnimation.h"

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