#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdarg.h>
#include <windows.h>
#include <GL\glut.h>
#include "AEUtility.h"
#include "AEFont.h"

AERect AEFont::getTexCoords(GLint imgOffset) {
	AERect rect;
	rect.x1 = 0.05 * (imgOffset % 20);
	rect.x2 = rect.x1 + 0.05;
	rect.y2 = 1.0 - 0.1 * (imgOffset / 20);
	rect.y1 = rect.y2 - 0.1;
	return rect;
}

GLvoid AEFont::build() {
	listBase = glGenLists(200);
	glBindTexture(GL_TEXTURE_2D, tex);
	for (GLint ascii = 0; ascii < 200; ascii++) {
		AERect rect = getTexCoords(ascii);
		glNewList(listBase + ascii, GL_COMPILE);
			glBegin(GL_QUADS);
				glTexCoord2f(rect.x1, rect.y1);
				glVertex2i(0, 0);
				glTexCoord2f(rect.x2, rect.y1);
				glVertex2i(fontW, 0);
				glTexCoord2f(rect.x2, rect.y2);
				glVertex2i(fontW, fontH);
				glTexCoord2f(rect.x1, rect.y2);
				glVertex2i(0, fontH);
			glEnd();
			glTranslated(10, 0, 0);
		glEndList();
	}
}

GLvoid AEFont::raze() {
	glDeleteLists(listBase, 200);
}

GLvoid AEFont::writeAt(GLint x, GLint y, const char* _Format, ...) {
	glBindTexture(GL_TEXTURE_2D, tex);
	char text[256];
	va_list ap;
	if (_Format == NULL)
		return;
	va_start(ap, _Format);
	vsprintf(text, _Format, ap);
	va_end(ap);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 640, 0, 480, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslated(x, y, 0);
	glListBase(listBase);
	glCallLists(strlen(text), GL_BYTE, text);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

GLvoid AEFont::writeColoredAt(GLint x, GLint y, GLfloat r, GLfloat g, GLfloat b, const char* _Format, ...) {
	glColor3f(r, g, b);
	glBindTexture(GL_TEXTURE_2D, tex);
	char text[256];
	va_list ap;
	if (_Format == NULL)
		return;
	va_start(ap, _Format);
	vsprintf(text, _Format, ap);
	va_end(ap);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 640, 0, 480, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslated(x, y, 0);
	glListBase(listBase);
	glCallLists(strlen(text), GL_BYTE, text);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glColor3f(1.0, 1.0, 1.0);
}

AEFontLibrary::AEFontLibrary() {
	maxIndex = 0;
	for (int i = 0; i < MAX_FONT_COUNT; i++) {
		lib[i] = NULL;
	}
}

GLvoid AEFontLibrary::add(AEFont* font) {
	if (maxIndex > MAX_FONT_COUNT) {
		// Error
		return;
	}
	lib[maxIndex] = font;
	maxIndex++;
}