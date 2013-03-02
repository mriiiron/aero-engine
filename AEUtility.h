/*******************************************************************
== File Info ==
Name:				util.h
Author:				CaiyZ (Mr.IroN)

== File Comment ==
This file contains items that are commonly used in other files.
*******************************************************************/

#pragma once

#define NULL					0

#define MAX_CHAR_COUNT_IN_LINE	100

#define INVERSE_X				0
#define INVERSE_Y				1

typedef struct _Rect {
	GLfloat x1, y1, x2, y2;
} AERect;

typedef struct _BiasRect {
	GLfloat x1, y1, x2, y2, x3, y3, x4, y4;
} AEBiasRect;

typedef struct _Point {
	GLfloat x, y;
} AEPoint;

GLint randomIntBetween(GLint start, GLint end);
AEPoint createPoint(GLfloat _x, GLfloat _y);
AERect createRect(GLfloat _x1, GLfloat _y1, GLfloat _x2, GLfloat _y2);
AERect getInversedRect(AERect r, GLint direction);
AEBiasRect getInversedRect(AEBiasRect r, GLint direction);
GLvoid paintRect(GLint tex, AERect texClip, AERect paintArea);
GLvoid paintRect(GLint tex, AERect texClip, AEBiasRect paintArea);
GLvoid paintRectByCenter(GLint tex, AERect texClip, AEPoint center, GLfloat width, GLfloat height, GLfloat angleDeg);