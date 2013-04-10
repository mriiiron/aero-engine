#include <GL\glut.h>
#include <sstream>
#include "AEroEngine.h"
#include "SPScene.h"
#include "SPLightning.h"
#include "SPParticleEffect.h"
#include "SPCaster.h"

extern AEObjectTable oTable;
extern AEParticleSystem ptclSys;

GLvoid AEFrame::addOptionalByStrAt(string line) {
	istringstream iss;
	string item;
	iss.clear();  iss.str(line);
	iss >> item;
	if (item == "$cast") {
		iss >> item;  GLint castPointX = stoi(item);
		iss >> item;  GLint castPointY = stoi(item);
		optionalParam[SPCastParam::slotIndex] = new SPCastParam(castPointX, castPointY);
	}
	// else if ..
}


SPCaster::SPCaster(AEScene* _scene, GLint _oid, GLint _team, GLfloat _cx, GLfloat _cy, GLint _action, GLint inverse): AESprite(_scene, _oid, _team, _cx, _cy, _action, inverse) {

}

GLvoid SPCaster::update() {
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
		SPCastParam* cast = (SPCastParam*)(anim->getFrame(frame)->getOptional(SPCastParam::slotIndex));
		if (cast != NULL) {
			castChainLightning(cast->getCastPoint(), target->getCenter());
			target->changeAction(2);
		}
	}
}

GLvoid SPCaster::castChainLightning(AEPoint gunpoint, AEPoint target) {
	AEFrame* f = oTable.get(oid)->getAnim(action)->getFrame(frame);
	AEPoint castPoint = AEUtil::createPoint(cx - f->getCenterx() + gunpoint.x, cy - f->getCentery() + gunpoint.y);
	((SPMainScene*)scene)->getSpellTable()->add(new SPLightning(castPoint, target, 1, 30, 8.0f));
	ptclSys.generate(new SPElectricShock(target.x, target.y, 0.0f, 100));
}