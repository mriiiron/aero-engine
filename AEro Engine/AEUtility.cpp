#include <GL\glut.h>
#include "AEUtility.h"

AEVector2 AEVector2::normalize(AEVector2 v) {
	return AEVector2(v.x / v.model(), v.y / v.model());
}

GLfloat AEVector2::cross(AEVector2 v1, AEVector2 v2) {
	return v1.x * v2.y - v1.y * v2.x;
}

GLfloat AEVector2::dotProduct(AEVector2 v1, AEVector2 v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

GLfloat AEVector2::angleBetween(AEVector2 v1, AEVector2 v2) {
	GLfloat angleValue = acos(AEVector2::dotProduct(v1, v2) / (v1.model() * v2.model()));
	if (AEVector2::cross(v1, v2) > 0) {
		return angleValue;
	}
	else {
		return -angleValue;
	}
}

AERect AEUtil::getInversedRect(AERect r, GLint direction) {
	AERect ir;
	if (direction == AEUtil::INVERSE_X) {
		ir.x1 = r.x2;
		ir.y1 = r.y1;
		ir.x2 = r.x1;
		ir.y2 = r.y2;
	}
	else if (direction == AEUtil::INVERSE_Y) {
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

AEBiasRect AEUtil::getInversedRect(AEBiasRect r, GLint direction) {
	AEBiasRect ir;
	if (direction == AEUtil::INVERSE_X) {
		ir.x1 = r.x2;
		ir.y1 = r.y2;
		ir.x2 = r.x1;
		ir.y2 = r.y1;
		ir.x3 = r.x4;
		ir.y3 = r.y4;
		ir.x4 = r.x3;
		ir.y4 = r.y3;
	}
	else if (direction == AEUtil::INVERSE_Y) {
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

GLvoid AEUtil::paintRect(GLint tex, AERect texClip, AERect paintArea) {
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

GLvoid AEUtil::paintRect(GLint tex, AERect texClip, AEBiasRect paintArea) {
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

GLvoid AEUtil::paintRectByCenter(GLint tex, AERect texClip, AEPoint center, GLfloat width, GLfloat height, GLfloat angleDeg) {
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