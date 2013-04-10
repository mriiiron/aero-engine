#include <GL\glut.h>
#include <cmath>
#include "AEroEngine.h"
#include "TDTurret.h"
#include "TDAI.h"

TDTurretAI::TDTurretAI(AESprite* _self): AEAI(_self) {

}

GLvoid TDTurretAI::execute() {
	AEScene* scene = self->getScene();
	AESpriteTable* sTable = scene->getSpriteTable();
	if (target == NULL) {
		// Search for a target
		for (GLint i = 0; i < sTable->getHashCount(); i++) {
			AESprite* sp = sTable->getByHash(i);
			if (sp->getTeam() == AESprite::TEAM_2 && sp->getState() != AEObject::STATE_PROJ_DESTROYED) {
				setTarget(sp);
				((TDTurret*)self)->setTarget(sp);
				return;
			}
		}
	}
	else {
		if (target->getHP() <= 0) {
			target = NULL;
			((TDTurret*)self)->stopFire();
			return;
		}
		GLfloat x1 = self->getCx(), x2 = target->getCx();
		GLfloat y1 = self->getCy(), y2 = target->getCy();
		AEVector2 faceVect = self->getFaceVector();
		GLfloat angleBetw = AEVector2::angleBetween(faceVect, AEVector2(x2 - x1, y2 - y1));
		GLfloat deg = abs(AEUtil::rad2deg(angleBetw));
		if (deg < 2.0f) {
			// shoot
			self->setVAngle(0.0f);
			((TDTurret*)self)->aimAccuratelyAt(target->getCx(), target->getCy());
			((TDTurret*)self)->openFire();
		}
		else {
			// chase
			if (angleBetw > 0.0f) {
				self->setVAngle(AEUtil::deg2rad(1.5f));
			}
			else if (angleBetw < 0.0f) {
				self->setVAngle(AEUtil::deg2rad(-1.5f));
			}
			((TDTurret*)self)->stopFire();
		}
	}

}