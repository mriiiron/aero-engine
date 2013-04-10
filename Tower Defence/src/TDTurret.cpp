#include <GL\glut.h>
#include <sstream>
#include "AEroEngine.h"
#include "TDTurret.h"

extern AEObjectTable oTable;

GLvoid AEFrame::addOptionalByStrAt(string line) {
	istringstream iss;
	string item;
	iss.clear();  iss.str(line);
	iss >> item;
	if (item == "$shoot") {
		iss >> item;  GLint damage = stoi(item);
		optionalParam[TDShootParam::slotIndex] = new TDShootParam(damage);
	}
	// else if ..
}

TDTurret::TDTurret(AEScene* _scene, GLint _oid, GLint _team, GLfloat _cx, GLfloat _cy, GLint _action, GLint inverse): AESprite(_scene, _oid, _team, _cx, _cy, _action, inverse) {
	target = NULL;
}

GLvoid TDTurret::aimAccuratelyAt(GLfloat x, GLfloat y) {
	if (x == cx) {
		if (y > cy) {
			angle = AEUtil::deg2rad(90.0);
		}
		else {
			angle = AEUtil::deg2rad(-90.0);
		}
	}
	else {
		angle = atan((y - cy) / (x - cx));
		if (x < cx) {
			if (y > cy) {
				angle -= 3.14159265f;
			}
			else {
				angle += 3.14159265f;
			}
		}
	}
}

GLvoid TDTurret::openFire() {
	if (state == STATE_IDLE) {
		changeAction(1);
	}
}

GLvoid TDTurret::stopFire() {
	if (state == STATE_FIRE) {
		changeAction(0);
	}
}

GLvoid TDTurret::update() {
	AEAnimation* anim = oTable.get(oid)->getAnim(action);
	if (timeToLive == 0) {
		changeAction(anim->getNext());
		return;
	}
	if (timeToLive > 0) timeToLive--;
	GLint fac = facing ? -1 : 1;
	cx += (fac * vx);
	cy += vy;
	vx += ax;
	vy += ay;
	angle += vangle;
	if (angle < -3.14159265f) angle += 2 * 3.14159265f;
	if (angle > 3.14159265f) angle -= 2 * 3.14159265f;
	time++;
	if (time >= anim->getEndTime(frame)) {
		cx += (fac * anim->getFrame(frame)->getShiftx());
		cy += anim->getFrame(frame)->getShifty();
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
		// ¹¶ÇÒ
		TDShootParam* shoot = (TDShootParam*)(anim->getFrame(frame)->getOptional(TDShootParam::slotIndex));
		if (shoot != NULL) {
			target->takeDamage(shoot->getDamage());
			AEPoint selfC = getCenter(), targetC = target->getCenter();
			AEVector2 aim = AEVector2::normalize(getFaceVector());
			GLfloat dist = AEUtil::calcDistance(selfC, targetC) - 20.0f;
			AEPoint hit = AEUtil::createPoint(selfC.x + dist * aim.x, selfC.y + dist * aim.y);
			scene->getSpriteTable()->add(new AESprite(scene, 99, AESprite::TEAM_NONE, hit.x, hit.y, 0));
		}
		// ¹¶ÇÒ½áÊø
	}
}