#pragma once


class TDTank: public AESprite {

public:

	static const GLint STATE_MOVE			= 1000;
	static const GLint STATE_DESTROYED		= 1002;

	TDTank(AEScene* _scene, GLint _oid, GLint _team, GLfloat _cx, GLfloat _cy, GLint _action = 0, GLint inverse = 0);

	GLvoid update();

};