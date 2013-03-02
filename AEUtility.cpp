#include <stdlib.h>
#include <math.h>
#include <GL\glut.h>
#include "AEUtility.h"

char* context = NULL;

GLint randomIntBetween(GLint start, GLint end) {
    return (rand() % (end - start + 1)) + start;
}

AEPoint createPoint(GLfloat _x, GLfloat _y) {
	AEPoint p;
	p.x = _x;
	p.y = _y;
	return p;
}

AERect createRect(GLfloat _x1, GLfloat _y1, GLfloat _x2, GLfloat _y2) {
	AERect r;
	r.x1 = _x1;
	r.y1 = _y1;
	r.x2 = _x2;
	r.y2 = _y2;
	return r;
}

AERect getInversedRect(AERect r, GLint direction) {
	AERect ir;
	if (direction == INVERSE_X) {
		ir.x1 = r.x2;
		ir.y1 = r.y1;
		ir.x2 = r.x1;
		ir.y2 = r.y2;
	}
	else if (direction == INVERSE_Y) {
		ir.x1 = r.x1;
		ir.y1 = r.y2;
		ir.x2 = r.x2;
		ir.y2 = r.y1;
	}
	else {
		// Error
	}
	return ir;
}

AEBiasRect getInversedRect(AEBiasRect r, GLint direction) {
	AEBiasRect ir;
	if (direction == INVERSE_X) {
		ir.x1 = r.x2;
		ir.y1 = r.y2;
		ir.x2 = r.x1;
		ir.y2 = r.y1;
		ir.x3 = r.x4;
		ir.y3 = r.y4;
		ir.x4 = r.x3;
		ir.y4 = r.y3;
	}
	else if (direction == INVERSE_Y) {
		ir.x1 = r.x4;
		ir.y1 = r.y4;
		ir.x2 = r.x3;
		ir.y2 = r.y3;
		ir.x3 = r.x2;
		ir.y3 = r.y2;
		ir.x4 = r.x1;
		ir.y4 = r.y1;
	}
	else {
		// Error
	}
	return ir;
}

GLvoid paintRect(GLint tex, AERect texClip, AERect paintArea) {
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);
		glTexCoord2f(texClip.x1, texClip.y1);
		glVertex2f(paintArea.x1, paintArea.y1);
		glTexCoord2f(texClip.x2, texClip.y1);
		glVertex2f(paintArea.x2, paintArea.y1);
		glTexCoord2f(texClip.x2, texClip.y2);
		glVertex2f(paintArea.x2, paintArea.y2);
		glTexCoord2f(texClip.x1, texClip.y2);
		glVertex2f(paintArea.x1, paintArea.y2);
	glEnd();
}

GLvoid paintRect(GLint tex, AERect texClip, AEBiasRect paintArea) {
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);
		glTexCoord2f(texClip.x1, texClip.y1);
		glVertex2f(paintArea.x1, paintArea.y1);
		glTexCoord2f(texClip.x2, texClip.y1);
		glVertex2f(paintArea.x2, paintArea.y2);
		glTexCoord2f(texClip.x2, texClip.y2);
		glVertex2f(paintArea.x3, paintArea.y3);
		glTexCoord2f(texClip.x1, texClip.y2);
		glVertex2f(paintArea.x4, paintArea.y4);
	glEnd();
}

GLvoid paintRectByCenter(GLint tex, AERect texClip, AEPoint center, GLfloat width, GLfloat height, GLfloat angleDeg) {
	GLfloat angle = angleDeg / 180.0 * 3.14159;
	GLfloat cosA = cos(angle), sinA = sin(angle);
	GLfloat halfHeight = height * 0.5, halfWidth = width * 0.5;
	AEBiasRect paintArea;
	paintArea.x1 = center.x + halfHeight * sinA - halfWidth * cosA;
	paintArea.y1 = center.y - halfHeight * cosA - halfWidth * sinA;
	paintArea.x2 = paintArea.x1 + width * cosA;
	paintArea.y2 = paintArea.y1 + width * sinA;
	paintArea.x3 = paintArea.x2 - height * sinA;
	paintArea.y3 = paintArea.y2 + height * cosA;
	paintArea.x4 = paintArea.x1 - height * sinA;
	paintArea.y4 = paintArea.y1 + height * cosA;
	paintRect(tex, texClip, paintArea);
}