#pragma once


class SPEnemy: public AESprite {

public:

	static const GLint STATE_IDLE			= 0;
	static const GLint STATE_ACT			= 1;
	static const GLint STATE_INJURED		= 2;
	static const GLint STATE_FALL			= 3;
	static const GLint STATE_LYING			= 99;
	static const GLint STATE_AIR_IDLE		= 100;
	static const GLint STATE_AIR_ACT		= 101;
	static const GLint STATE_AIR_INJURED	= 102;

	SPEnemy(AEScene* _scene, GLint _oid, GLint _team, GLfloat _cx, GLfloat _cy, GLint _action = 0, GLint inverse = 0);
	GLvoid update();

private:



};