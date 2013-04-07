#pragma once


class SPElectricParticle: public AEParticle {

public:

	GLvoid init();
	GLvoid update();
	GLvoid paint();
	GLvoid reset();

private:



};


class SPElectricShock: public AEParticleEmitter {

public:

	SPElectricShock(GLfloat _x, GLfloat _y, GLfloat _z = 0.0, GLint _count = 1, GLint _life = 100);
	GLvoid update();

private:



};