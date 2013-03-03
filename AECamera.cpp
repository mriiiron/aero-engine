#include <GL\glut.h>
#include "AEUtility.h"
#include "AEObject.h"
#include "AESprite.h"
#include "AECamera.h"

#define NULL					0

extern AESpriteTable sTable;
extern AEObjectTable oTable;

GLvoid AECamera::init() {
	mode = MODE_STABLE;
	crosshair.x = crosshair.y = 0.0;
	vx = vy = 0.0;
	keyLeftDown = keyRightDown = 0;
	target = NULL;
}

GLvoid AECamera::move() {
	crosshair.x += vx;
	crosshair.y += vy;
	glTranslatef(-vx, -vy, 0.0);
}

GLvoid AECamera::update() {
	GLfloat distX;
	switch (mode) {
	case MODE_STABLE:
		break;
	case MODE_CHASE:
		distX = target->getCx() - crosshair.x;
		GLint fac;
		if (target->getFacing() == AESprite::FACING_RIGHT)
			fac = 1;
		else
			fac = -1;
		if (distX < -50.0) {
			vx = (distX + 50.0) * CAM_CHASE_SPEED + fac * target->getVx();
		}
		else if (distX > 50.0) {
			vx = (distX - 50.0) * CAM_CHASE_SPEED + fac * target->getVx();
		}
		else {
			vx = 0.0;
		}
		move();
		break;
	case MODE_KEYBOARD:
		vx = 2.5 * (keyRightDown - keyLeftDown);
		move();
		break;
	default:
		break;
	}
}

GLvoid AECamera::changeTarget() {
	if (mode != MODE_CHASE)
		return;
	GLint targetIndex = target->getIndex();
	do {
		targetIndex++;
		if (targetIndex >= sTable.getHashCount())
			targetIndex = 0;
	} while (oTable.get(sTable.getByHash(targetIndex)->getOid())->getType() != OBJ_CHARACTER);
	setTarget(sTable.getByHash(targetIndex));
}