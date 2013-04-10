#pragma once

#include "SPEnemy.h"


class SPCastParam: public AEFrameOptional {

public:

	static const GLint slotIndex		 = 0;

	SPCastParam(GLint _castPointX, GLint _castPointY) { castPoint = AEUtil::createPoint(_castPointX, _castPointY); }

	AEPoint getCastPoint() { return castPoint; }

private:

	AEPoint castPoint;

};


class SPCaster: public AESprite {

public:

	SPCaster(AEScene* _scene, GLint _oid, GLint _team, GLfloat _cx, GLfloat _cy, GLint _action = 0, GLint inverse = 0);

	GLvoid setTarget(SPEnemy* _target) { target = _target; }
	GLvoid update();

	GLvoid castChainLightning(AEPoint gunpoint, AEPoint target);
	GLvoid castThunderStrike();

private:

	SPEnemy* target;

};