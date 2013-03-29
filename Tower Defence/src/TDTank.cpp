#include <GL\glut.h>
#include "AEroEngine.h"
#include "TDTank.h"

TDTank::TDTank(AEScene* _scene, GLint _oid, GLint _team, GLfloat _cx, GLfloat _cy, GLint _action, GLint inverse): AESprite(_scene, _oid, _team, _cx, _cy, _action, inverse) {
}

GLvoid TDTank::update() {
	if (hpValue <= 0 && state != STATE_DESTROYED) {
		changeAction(10);
		return;
	}
	AESprite::update();
}