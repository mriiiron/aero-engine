#include <GL\glut.h>
#include "AEroEngine.h"
#include "SPEnemy.h"

extern AEObjectTable oTable;

SPEnemy::SPEnemy(AEScene* _scene, GLint _oid, GLint _team, GLfloat _cx, GLfloat _cy, GLint _action, GLint inverse): AESprite(_scene, _oid, _team, _cx, _cy, _action, inverse) {

}

GLvoid SPEnemy::update() {
	AEBackground* bg = scene->getBackground();
	AEAnimation* anim = oTable.get(oid)->getAnim(action);
	if (timeToLive == 0) {
		changeAction(anim->getNext());
		return;
	}
	if (timeToLive > 0) timeToLive--;
	GLint fac;
	if (facing) fac = -1; else fac = 1;
	cx += (fac * vx);
	cy += vy;
	vx += ax;
	vy += ay;
	if (angle < -3.14159265f) angle += 2 * 3.14159265f;
	if (angle > 3.14159265f) angle -= 2 * 3.14159265f;
	if (drop > 0) {
		gndSpeed = 0.0f;
		if (drop + vy <= 0) {
			drop = 0;
			cy = bg->getLocation().y + bg->getLandform(onLandform).data[bg->getXonBG(GLint(cx))].altitude;
			if (state == STATE_FALL) {
				changeAction(action + 1);
				vy = ay = 0.0f;
			}
			else {
				vy = ay = 0.0f;
			}
		}
		else {
			onLandform = bg->getLandformIndexBelow(GLint(cx), GLint(cy), &drop);
			if (state < 100) {
				ay = AEPhysics::GRAVITY;
			}
		}
	}
	else {
		onLandform = bg->getLandformIndexBelow(GLint(cx), GLint(cy), &drop);
		if (abs(drop) <= AEBackground::ONLANDFORM_TOLERANCE) {
			drop = 0;
			cy = bg->getLocation().y + bg->getLandform(onLandform).data[bg->getXonBG(GLint(cx))].altitude;
			GLfloat slope = bg->getLandform(onLandform).data[bg->getXonBG(GLint(cx))].slope;
			GLfloat string = sqrt(1.0f + slope * slope);
			vx = gndSpeed * 1.0f / string;
			vy = gndSpeed * fac * slope / string;
		}
		else {
			if (state < 100) {
				ay = AEPhysics::GRAVITY;
			}
		}
	}
	time++;
	if (time >= anim->getEndTime(frame)) {
		cx += (fac * anim->getFrame(frame)->getShiftx());
		cy += anim->getFrame(frame)->getShiftx();
		frame++;
		if (time >= anim->getEndTime(anim->getFrameCount() - 1)) {
			time = 0;
		}
		if (frame == anim->getFrameCount()) {
			frame = 0;
			if (!anim->isLoop()) {
				changeAction(anim->getNext());
				return;
			}
		}
		GLint dvx = anim->getFrame(frame)->getDvx();
		GLint dvy = anim->getFrame(frame)->getDvy();
		if (drop > 0) {
			gndSpeed = (dvx == 999 ? 0.0f : gndSpeed + dvx);
		}
		else {
			vx = (dvx == 999 ? 0.0f : vx + dvx);
		}
		vy = (dvy == 999 ? 0.0f : vy + dvy);
	}
}