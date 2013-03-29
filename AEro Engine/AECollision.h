#pragma once


class AECollisionResult {

public:

	static const GLint TYPE_NONE				= 0;
	static const GLint TYPE_NORMAL				= 1;

	AECollisionResult(GLint _type) { type = _type; }
	AECollisionResult(GLint _type, AEPoint _point) { type = _type;  point = _point; }

	GLint type;
	AEPoint point;

};


class AECollision {

public:

	const static GLint HIT_NORMAL				= 0;
	const static GLint HIT_HEAVY				= 1;
	const static GLint HIT_LIGHT				= 2;
	const static GLint HIT_CRITICAL				= 3;

	static AECollisionResult check(AESprite* s1, AESprite* s2, Frame* f1, Frame* f2);
	static AECollisionResult ray(AESprite* s1, AESprite* s2, Frame* f1);
	static GLvoid handle(AESprite* s1, AESprite* s2, Frame* f1, Frame* f2, AEPoint sparkPos);

private:



};