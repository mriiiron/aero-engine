#include <GL\glut.h>
#include "AeroEngine.h"
#include "SPLightning.h"

SPLightning::SPLightning(AEPoint caster, AEPoint target, GLint _stripCount, GLint _segmentCount, GLfloat _nodeRadius) {
	stripCount = _stripCount;  segmentCount = _segmentCount;  nodeRadius = _nodeRadius;
	time = 0.0f;  alpha = 1.0f;
	ltnIndexes = new GLushort[2 * segmentCount];
	for (GLint i = 0; i < 2 * segmentCount; i++) ltnIndexes[i] = i;
	ltnVertices = new GLfloat[4 * segmentCount];
	ltnVertices[0] = caster.x;  ltnVertices[1] = caster.y;
	ltnVertices[4 * segmentCount - 2] = target.x;  ltnVertices[4 * segmentCount - 1] = target.y;
	GLfloat stepLength = AEUtil::calcDistance(caster, target) / segmentCount;
	GLfloat x = caster.x, y = caster.y;
	GLfloat xStep = (target.x - caster.x) / segmentCount, yStep = (target.y - caster.y) / segmentCount;
	for (GLint i = 0; i < segmentCount - 1; i++) {
		x += xStep;  y += yStep;
		GLfloat nodeX = x + stepLength * (2 * AEUtil::randomClampf() - 1);
		GLfloat nodeY = y + stepLength * (2 * AEUtil::randomClampf() - 1);
		ltnVertices[4 * i + 2] = nodeX;  ltnVertices[4 * i + 3] = nodeY;
		ltnVertices[4 * i + 4] = nodeX + nodeRadius * (2 * AEUtil::randomClampf() - 1);
		ltnVertices[4 * i + 5] = nodeY + nodeRadius * (2 * AEUtil::randomClampf() - 1);
	}
}

GLvoid SPLightning::update() {
	if (time >= 1.0f) {
		deadFlag = GL_TRUE;
		return;
	}
	time += 0.1f;
	alpha = (1 - time) * (1 - time);
}

GLvoid SPLightning::paint() {
	glColor4f(1.0f, 1.0f, 1.0f, alpha);
	glVertexPointer(2, GL_FLOAT, 0, ltnVertices);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * segmentCount);
	// or using: glDrawElements(GL_TRIANGLE_STRIP, 2 * segmentCount, GL_UNSIGNED_SHORT, ltnIndexes);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}