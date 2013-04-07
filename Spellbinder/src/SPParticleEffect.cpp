#include <GL\glut.h>
#include "AeroEngine.h"
#include "SPParticleEffect.h"

extern AEParticleSystem ptclSys;

GLvoid SPElectricParticle::init() {
	cx = matrixEmit->getX();  cy = matrixEmit->getY();  cz = 0.0f;
	life = 1.0f;  fade = 0.08f;
	GLfloat _v = GLfloat(AEUtil::randomIntBetween(15, 20));
	GLfloat _thetaDeg = AEUtil::randomIntBetween(-45, 45);
	GLfloat _gammaDeg = AEUtil::randomIntBetween(-25, 25);
	if (_gammaDeg < 0) {
		_thetaDeg += 180;
		_gammaDeg = - _gammaDeg;
	}
	GLfloat _theta = AEUtil::deg2rad(_thetaDeg);
	GLfloat _gamma = AEUtil::deg2rad(_gammaDeg);
	vx = _v * cos(_theta) * cos(_gamma);
	vy = _v * sin(_gamma);
	vz = _v * sin(_theta) * cos(_gamma);
	ax = ay = az = 0.0f;
	r = g = b = 1.0f;
	damping = 1.2f;
	angle = vrotate = 0.0f;
}

GLvoid SPElectricParticle::reset() {
	disable();
	if (attachEmit != NULL) {
		delete attachEmit;
		attachEmit = NULL;
	}
}

GLvoid SPElectricParticle::update() {
	cx += vx;  cy += vy;  cz += vz;
	vx = vx / damping + ax;  vy = vy / damping + ay;  vz = vz / damping + az;
	life -= fade;
	if (attachEmit != NULL) {
		attachEmit->setPosition(cx, cy, cz);
		attachEmit->update();
	}
	if (life < 0.0) {
		reset();
	}
}

GLvoid SPElectricParticle::paint() {
	GLfloat alpha = 1.0 - (1.0 - life) * (1.0 - life) * (1.0 - life) * (1.0 - life);
	GLfloat size = 1.0 - (1.0 - life) * (1.0 - life);
	glDisable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, alpha);
	glPointSize(3.0f * size);
	glBegin(GL_POINTS);
		glVertex3f(cx, cy, cz);
	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	if (attachEmit != NULL) {
		attachEmit->paint();
	}
}

SPElectricShock::SPElectricShock(GLfloat _x, GLfloat _y, GLfloat _z, GLint _count, GLint _life): AEParticleEmitter(_x, _y, _z, _count, _life) {
	for (GLint i = 0; i < count; i++) {
		group[i] = new SPElectricParticle();
		group[i]->setMatrixEmitter(this);
		group[i]->init();
		group[i]->enable();
	}
}

GLvoid SPElectricShock::update() {
	GLboolean deadFlag = GL_TRUE;
	for (GLint i = 0; i < count; i++) {
		if (group[i]->isEnabled()) {
			group[i]->update();
			deadFlag = GL_FALSE;
		}
	}
	if (deadFlag) {
		ptclSys.remove(index);
	}
}