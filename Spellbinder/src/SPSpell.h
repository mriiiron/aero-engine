#pragma once


class SPSpell {

public:

	virtual GLvoid update() = 0;
	virtual GLvoid paint() = 0;

	SPSpell() { deadFlag = GL_FALSE; }
	GLboolean isDead() { return deadFlag; }

protected:

	GLboolean deadFlag;

};