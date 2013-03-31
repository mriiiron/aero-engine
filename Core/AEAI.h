/**
 * @file	AEAI.h
 * @brief	Includes the definations of AI.
 *
 * The AI in AEro Engine is just like a "soul" to the sprite. Imagine that when a 
 * sprite losts its AI, it would has no perception about its surroundings, thus
 * it would just act as a walking dead.
 *
 * Q: What kinds of Sprite need an AI?
 * A: Enemy fighters, automatic turrets, tracking missiles, etc.
 *
 * Q: For what AI is unnecessary?
 * A: Bullets, energy balls, falling leaves, etc.
 *
 * @author	CaiyZ (Mr.IroN)
 * @version	0.1
 * @date	2013
*/

#pragma once

class AEAI {

public:

	static const GLint EXEC_INTERVAL			= 3;

	AEAI(AESprite* _self);
	GLvoid releaseAllKeys();
	GLvoid setTarget(AESprite* _target) { target = _target; }
	GLvoid enable() { enabled = GL_TRUE; }
	GLvoid disable() { enabled = GL_FALSE; }
	GLvoid setIndex(GLint _index) { index = _index; }
	GLbyte isEnabled() { return (enabled ? 1 : 0); }
	GLint checkdistXWith(AESprite* sp) { return abs(self->getCx() - sp->getCx()); }
	GLint checkdistYWith(AESprite* sp) { return abs(self->getCy() - sp->getCy()); }

	virtual GLvoid execute() = 0;

protected:

	AESprite* self;
	AESprite* target;
	GLboolean enabled;
	GLint index;

};

class AEAITable {

public:

	static const GLint MAX_ONLINE_AIS = 120;

	AEAITable();
	AEAI* get(GLint index) { return table[index]; }
	AEAI* getByHash(GLint hashIndex) { return table[hash[hashIndex]]; }
	GLint getHashCount() { return pHash; }
	GLvoid add(AEAI* ai);
	GLvoid addAt(GLint index, AEAI* sp);
	GLvoid remove(GLint index);
	GLvoid clear();
	GLvoid update();

private:

	AEAI* table[MAX_ONLINE_AIS];
	GLint occupied[MAX_ONLINE_AIS];
	GLint hash[MAX_ONLINE_AIS];
	GLint maxIndex, pHash;

};