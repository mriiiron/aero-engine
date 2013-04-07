#pragma once

#include "SPSpell.h"


class SPLightning: public SPSpell {

public:

	SPLightning(AEPoint caster, AEPoint target, GLint _stripCount, GLint _segmentCount, GLfloat _nodeRadius);
	GLvoid update();
	GLvoid paint();

private:

	GLint stripCount, segmentCount;
	GLfloat nodeRadius;
	GLfloat time;
	GLclampf alpha;
	GLushort* ltnIndexes;
	GLfloat* ltnVertices;

};