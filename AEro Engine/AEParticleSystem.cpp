#include <stdio.h>
#include <GL\glut.h>
#include <math.h>
#include "AEUtility.h"
#include "AEParticleSystem.h"

extern AEParticleSystem ptclSys;

AEParticle::AEParticle() {
	enabled = 0;
	matrixEmit = attachEmit = NULL;
}

GLvoid AEParticle::init() {
	tex = ptclSys.getTexture(0);
	cx = matrixEmit->getX();  cy = matrixEmit->getY();  cz = 0.0;
	GLint alt;
	GLfloat _v, _theta, _gamma;
	switch (matrixEmit->getType()) {
	case EMIT_TYPE_SINGLE:
		life = 1.0;  fade = 0.1;
		vx = vy = vz = 0.0;
		ax = ay = az = 0.0;
		// ay = -0.5;
		r = g = b = 1.0;
		damping = 1.0;
		angle = vrotate = 0.0;
		break;
	case EMIT_TYPE_EXPLODE:
		life = 1.0;  fade = 0.02;
		_v = AEUtil::randomIntBetween(15, 20) / 1.0;
		_theta = AEUtil::randomIntBetween(-45, 45) / 180.0 * 3.14159;
		_gamma = AEUtil::randomIntBetween(-180, 179) / 180.0 * 3.14159;
		vx = _v * cos(_theta) * cos(_gamma);
		vy = _v * cos(_theta) * sin(_gamma);
		vz = _v * sin(_theta) * sin(_gamma);
		ax = ay = az = 0.0;
		alt = AEUtil::randomIntBetween(0, 2);
		switch (alt) {
		case 0: r = 1.0;  g = 0.2;  b = 0.2;  break;
		case 1: r = 0.2;  g = 1.0;  b = 0.2;  break;
		case 2: r = 0.2;  g = 0.2;  b = 1.0;  break;
		default: r = 1.0;  g = 1.0;  b = 1.0;  break;
		}
		damping = 1.2;
		angle = vrotate = 0.0;
		break;
	default:
		// Error
		break;
	}
}

GLvoid AEParticle::reset() {
	switch (matrixEmit->getMode()) {
	case EMIT_MODE_ERUPT:
		disable();
		if (attachEmit != NULL) {
			delete attachEmit;
			attachEmit = NULL;
		}
		break;
	case EMIT_MODE_GRADUALLY:
		init();
		update();
		break;
	default:
		// Error
		break;
	}

}

GLvoid AEParticle::update() {
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

GLvoid AEParticle::paint(GLint method) {
	GLfloat alpha = 1.0 - (1.0 - life) * (1.0 - life) * (1.0 - life) * (1.0 - life);
	GLfloat size = 1.0 - (1.0 - life) * (1.0 - life);
	switch (method) {
	case PAINT_METHOD_TEXTURE:
		glColor4f(r, g, b, alpha);
		AEUtil::paintRectByCenter(tex, AEUtil::createRect(0.0, 0.0, 1.0, 1.0), AEUtil::createPoint(cx, cy), 16 * size, 16 * size, angle);
		break;
	case PAINT_METHOD_QUADRANGLE:
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_TRIANGLE_FAN);
			glColor4f(1.0, 1.0, 1.0, alpha);
			glVertex2f(cx, cy);
			glColor4f(r, g, b, 0.0);
			glVertex2f(cx + 8 * size, cy);
			glVertex2f(cx, cy + 8 * size);
			glVertex2f(cx - 8 * size, cy);
			glVertex2f(cx, cy - 8 * size);
			glVertex2f(cx + 8 * size, cy);
		glEnd();
		glEnable(GL_TEXTURE_2D);
		glColor4f(1.0, 1.0, 1.0, alpha);
		AEUtil::paintRectByCenter(tex, AEUtil::createRect(0.0, 0.0, 1.0, 1.0), AEUtil::createPoint(cx, cy), 8 * size, 8 * size, angle);
		break;
	case PAINT_METHOD_HEXAGON:

		break;
	case PAINT_METHOD_OCTAGON:

		break;
	default:
		// Error
		break;
	}
	if (attachEmit != NULL) {
		attachEmit->paint();
	}
}

AEParticleEmitter::AEParticleEmitter(GLint _type, GLint _mode, GLfloat _x, GLfloat _y, GLfloat _z, GLint _count, GLint _life) {
	for (GLint i = 0; i < MAX_EMITTER_PARTICLES; i++) {
		group[i].disable();
		group[i].setMatrixEmitter(this);
	}
	type = _type;  mode = _mode;  count = _count;
	setPosition(_x, _y, _z);
	
	// Adding a Tail (To be modified)
	//for (GLint i = 0; i < count; i++) {
	//	if (type == EMIT_TYPE_EXPLODE)
	//		group[i].setAttachEmitter(new ParticleEmitter(EMIT_TYPE_SINGLE, EMIT_MODE_GRADUALLY, _x, _y, _z, 15));
	//}

	switch (mode) {
	case EMIT_MODE_ERUPT:
		for (GLint i = 0; i < count; i++) {
			group[i].init();
			group[i].enable();
		}
		break;
	case EMIT_MODE_GRADUALLY:
		setLife(_life);
		resetTime();
		break;
	default:
		break;
	}
}

GLvoid AEParticleEmitter::update() {
	GLint deadFlag = 1;
	switch (mode) {
	case EMIT_MODE_ERUPT:
		for (GLint i = 0; i < count; i++) {
			if (group[i].isEnabled()) {
				group[i].update();
				deadFlag = 0;
			}
		}
		if (deadFlag) {
			ptclSys.remove(index);
		}
		break;
	case EMIT_MODE_GRADUALLY:
		if (life >= 0) {
			if (time < count) {
				group[time].init();
				group[time].enable();
			}
			for (GLint i = 0; i < count; i++) {
				if (group[i].isEnabled()) {
					group[i].update();
				}
			}
			time++;
			if (time >= life) {
				mode = EMIT_MODE_ERUPT;
			}
		}
		break;
	default:
		break;
	}
}

GLvoid AEParticleEmitter::paint() {
	for (GLint i = 0; i < MAX_EMITTER_PARTICLES; i++) {
		if (group[i].isEnabled()) {
			group[i].paint();
		}
	}
}

AEParticleSystem::AEParticleSystem() {
	maxIndex = -1;
	for (GLint i = 0; i < MAX_ONLINE_GENERATORS; i++) {
		emitters[i] = NULL;
		occupied[i] = 0;
	}
}

GLvoid AEParticleSystem::generate(AEParticleEmitter* emitter) {
	for (GLint i = 0; i <= maxIndex; i++) {
		if (!occupied[i]) {
			emitter->setIndex(i);
			emitters[i] = emitter;
			occupied[i] = 1;
			return;
		}
	}
	if (maxIndex == MAX_ONLINE_GENERATORS - 1) {
		printf("Too many particle generators online ..\n");
	}
	else {
		maxIndex++;
		emitter->setIndex(maxIndex);
		emitters[maxIndex] = emitter;
		occupied[maxIndex] = 1;
	}
}

GLvoid AEParticleSystem::remove(GLint index) {
	if (!occupied[index]) {
		printf("Error on removing particle generator: Slot Empty.\n");
		return;
	}
	delete emitters[index];
	emitters[index] = NULL;
	occupied[index] = 0;
	while (!occupied[maxIndex] && maxIndex > 0) maxIndex--;
}

GLvoid AEParticleSystem::update() {
	for (GLint i = 0; i <= maxIndex; i++) {
		if (occupied[i])
			emitters[i]->update();
	}
}

GLvoid AEParticleSystem::paint() {
	for (GLint i = 0; i <= maxIndex; i++) {
		if (occupied[i])
			emitters[i]->paint();
	}
}