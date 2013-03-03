#pragma once

#define CAM_CHASE_DIST			200.0
#define CAM_CHASE_SPEED			0.05

class AECamera {

public:

	static const GLint MODE_STABLE				= 0;
	static const GLint MODE_CHASE				= 1;
	static const GLint MODE_KEYBOARD			= 2;

	GLvoid init();
	GLvoid move();
	GLvoid update();
	GLvoid changeTarget();
	GLvoid setFocus(GLfloat _x, GLfloat _y) { crosshair.x = _x;  crosshair.y = _y; }
	GLvoid setVx(GLfloat _vx) { vx = _vx; }
	GLvoid setVy(GLfloat _vy) { vy = _vy; }
	GLvoid pressLeft() { keyLeftDown = 1; }
	GLvoid pressRight() { keyRightDown = 1; }
	GLvoid releaseLeft() { keyLeftDown = 0; }
	GLvoid releaseRight() { keyRightDown = 0; }
	GLvoid setMode(GLint _mode) { mode = _mode; }
	GLvoid setTarget(AESprite* _target) { target = _target; }
	GLvoid shiftCrossHair(GLfloat _shiftx, GLfloat _shifty) { crosshair.x += _shiftx;  crosshair.y += _shifty; }
	AEPoint getCrosshair() { return crosshair; }

private:

	GLint mode;
	AEPoint crosshair;
	GLfloat vx, vy;
	GLubyte keyLeftDown, keyRightDown;
	AESprite* target;

};