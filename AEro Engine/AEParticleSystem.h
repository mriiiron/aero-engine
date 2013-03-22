#pragma once

#define NULL						0

#define MAX_EMITTER_PARTICLES		200
#define MAX_ONLINE_GENERATORS		1000
#define MAX_PARTICLE_TYPES			50

#define EMIT_TYPE_SINGLE			0
#define EMIT_TYPE_EXPLODE			1
#define EMIT_TYPE_SPIT				2

#define EMIT_MODE_ERUPT				0
#define EMIT_MODE_GRADUALLY			1

#define PAINT_METHOD_TEXTURE		0
#define PAINT_METHOD_QUADRANGLE		1
#define PAINT_METHOD_HEXAGON		2
#define PAINT_METHOD_OCTAGON		3


class AEParticleEmitter;


class AEParticle {

public:

	AEParticle();
	GLvoid update();
	GLvoid paint(GLint method = PAINT_METHOD_QUADRANGLE);
	GLvoid init();
	GLvoid reset();
	GLvoid split();
	GLint isEnabled() { return enabled; }
	GLvoid setMatrixEmitter(AEParticleEmitter* _matrixEmit) { matrixEmit = _matrixEmit; }
	GLvoid setAttachEmitter(AEParticleEmitter* _attachEmit) { attachEmit = _attachEmit; }
	GLvoid disable() { enabled = 0; }
	GLvoid enable() { enabled = 1; }
	GLvoid setPosition(GLfloat _cx, GLfloat _cy, GLfloat _cz = 0.0) { cx = _cx;  cy = _cy;  cz = _cz; }
	GLvoid setDamping(GLfloat _damping) { damping = _damping; }
	GLvoid setVelocity(GLfloat _vx, GLfloat _vy, GLfloat _vz) { vx = _vx;  vy = _vy;  vz = _vz; }
	GLvoid setAcceleration(GLfloat _ax, GLfloat _ay, GLfloat _az) { ax = _ax;  ay = _ay;  az = _az; }
	GLvoid setFade(GLfloat _fade) { fade = _fade; }
	GLvoid setAngle(GLfloat _angle) { angle = _angle; }
	GLvoid setVRotate(GLfloat _vrotate) { vrotate = _vrotate; }
	GLvoid setTexture(GLint _tex) { tex = _tex; }
	GLvoid setRGB(GLfloat _r, GLfloat _g, GLfloat _b) { r = _r;  g = _g;  b = _b; }
	GLvoid setLife(GLfloat _life) { life = _life; }

private:

	GLint enabled;
	AEParticleEmitter* matrixEmit;
	AEParticleEmitter* attachEmit;
	GLfloat cx, cy, cz, vx, vy, vz, ax, ay, az;
	GLfloat damping;
	GLfloat angle, vrotate;
	GLfloat r, g, b;
	GLfloat life, fade;
	GLint tex;

};

class AEParticleEmitter {

public:

	AEParticleEmitter(GLint _type, GLint _mode, GLfloat _x, GLfloat _y, GLfloat _z = 0.0, GLint _count = 1, GLint _life = 100);
	GLvoid update();
	GLvoid paint();
	GLvoid setPosition(GLfloat _x, GLfloat _y, GLfloat _z = 0.0) { x = _x;  y = _y;  z = _z; }
	GLvoid setIndex(GLint _index) { index = _index; }
	GLvoid setLife(GLint _life) { life = _life; }
	GLvoid resetTime() { time = 0; }
	GLfloat getX() { return x; }
	GLfloat getY() { return y; }
	GLint getType() { return type; }
	GLint getMode() { return mode; }
	GLint getCount() { return count; }

private:

	GLint index;
	AEParticle group[MAX_EMITTER_PARTICLES];
	GLint type, mode;
	GLint count;
	GLfloat x, y, z;
	GLint time, life;

};


class AEParticleSystem {

public:

	AEParticleSystem();
	GLint getTexture(GLint index) { return texTable[index]; }
	GLvoid setTexture(GLint index, GLint _tex) { texTable[index] = _tex; }
	GLvoid generate(AEParticleEmitter* emitter);
	GLvoid remove(GLint index);
	GLvoid clear();
	GLvoid update();
	GLvoid paint();

private:

	AEParticleEmitter* emitters[MAX_ONLINE_GENERATORS];
	GLint occupied[MAX_ONLINE_GENERATORS];
	GLint texTable[MAX_PARTICLE_TYPES];
	GLint maxIndex;

};

// ptclSys.generate(EFFECT_EXPLODE, x, y)