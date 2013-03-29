#pragma once


class TDTurret: public AESprite {

public:

	static const GLint STATE_IDLE			= 1000;
	static const GLint STATE_FIRE			= 1001;

	TDTurret(AEScene* _scene, GLint _oid, GLint _team, GLfloat _cx, GLfloat _cy, GLint _action = 0, GLint inverse = 0);

	GLvoid setTarget(AESprite* _target) { target = _target; }
	GLvoid aimAccuratelytAt(GLfloat x, GLfloat y);
	GLvoid openFire();
	GLvoid stopFire();

	GLvoid update();

private:

	AESprite* target;

};