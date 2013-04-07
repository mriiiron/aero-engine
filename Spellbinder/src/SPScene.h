#pragma once

#include "SPSpell.h"


class SPTitleScene: public AEScene {

public:

	SPTitleScene();
	GLvoid update();
	GLvoid paint();
	GLvoid keyDown(GLubyte key, int x, int y);

private:

	GLint fadeInTimer, holdTimer, fadeOutTimer;
	GLfloat fadeInStep, fadeOutStep;
	GLfloat logoAlpha;

};


class SPMainScene: public AEScene {

public:

	GLvoid update();
	GLvoid paint();
	GLvoid keyDown(GLubyte key, int x, int y);

private:

	AEHashedTable<SPSpell, 200> spellTable;

};