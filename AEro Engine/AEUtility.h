/*******************************************************************
== File Info ==
Name:				util.h
Author:				CaiyZ (Mr.IroN)

== File Comment ==
This file contains items that are commonly used in other files.
*******************************************************************/

#pragma once

#include <stdlib.h>
#include <cmath>

#define NULL								0

typedef struct _Rect {
	GLfloat x1, y1, x2, y2;
} AERect;


typedef struct _BiasRect {
	GLfloat x1, y1, x2, y2, x3, y3, x4, y4;
} AEBiasRect;


typedef struct _Point {
	GLfloat x, y;
} AEPoint;


class AEVector2 {

public:

	AEVector2(GLfloat _x, GLfloat _y) { x = _x;  y = _y; }
	
	GLfloat model() { return sqrtf(x * x + y * y); }

	static AEVector2 normalize(AEVector2 v);
	static GLfloat cross(AEVector2 v1, AEVector2 v2);
	static GLfloat dotProduct(AEVector2 v1, AEVector2 v2);
	static GLfloat angleBetween(AEVector2 v1, AEVector2 v2);

	GLfloat x, y;

};


class AEUtil {

public:

	static const GLint INVERSE_X				= 0;
	static const GLint INVERSE_Y				= 1;

	static GLfloat deg2rad(GLfloat deg) { return deg / 180.0f * 3.14159f; }
	static GLfloat rad2deg(GLfloat rad) { return rad / 3.14159f * 180.0f; }

	static GLfloat calcDistance(AEPoint p1, AEPoint p2) { return sqrtf((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)); }
	static GLint randomIntBetween(GLint start, GLint end) { return (rand() % (end - start + 1)) + start; }
	static AEPoint createPoint(GLfloat _x, GLfloat _y) { AEPoint p;  p.x = _x;  p.y = _y;  return p; }
	static AERect createRect(GLfloat _x1, GLfloat _y1, GLfloat _x2, GLfloat _y2) { AERect r;  r.x1 = _x1;  r.y1 = _y1;  r.x2 = _x2;  r.y2 = _y2;  return r; }
	
	static AERect getInversedRect(AERect r, GLint direction);
	static AEBiasRect getInversedRect(AEBiasRect r, GLint direction);
	static GLvoid paintRect(GLint tex, AERect texClip, AERect paintArea);
	static GLvoid paintRect(GLint tex, AERect texClip, AEBiasRect paintArea);
	static GLvoid paintRectByCenter(GLint tex, AERect texClip, AEPoint center, GLfloat width, GLfloat height, GLfloat angleDeg);

};