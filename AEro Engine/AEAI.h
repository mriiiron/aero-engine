#pragma once

class AEAI {

public:

	static const GLint EXEC_INTERVAL			= 3;

	static const GLint TYPE_NONE				= 0;
	static const GLint TYPE_SWORDMAN			= 1;
	static const GLint TYPE_GUARD				= 2;
	static const GLint TYPE_SWORDMAN_TRAINER	= 3;

	AEAI(GLint _type, AESprite* _self);
	GLvoid execute();
	GLvoid releaseAllKeys();
	GLvoid setTarget(AESprite* _target) { target = _target; }
	GLvoid enable() { enabled = GL_TRUE; }
	GLvoid disable() { enabled = GL_FALSE; }
	GLvoid setIndex(GLint _index) { index = _index; }
	GLbyte isEnabled() { return (enabled ? 1 : 0); }
	GLint checkdistXWith(AESprite* sp) { return abs(self->getCx() - sp->getCx()); }
	GLint checkdistYWith(AESprite* sp) { return abs(self->getCy() - sp->getCy()); }

private:

	AESprite* self;
	AESprite* target;
	GLboolean enabled;
	GLint index, type;

};

class AEAITable {

public:

	static const GLint MAX_ONLINE_AIS = 120;
	static const GLint AI_COUNT = 5;

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