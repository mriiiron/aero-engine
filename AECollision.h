#pragma once


class AECollision {

public:

	const static GLint HIT_NORMAL				= 0;
	const static GLint HIT_HEAVY				= 1;
	const static GLint HIT_LIGHT				= 2;
	const static GLint HIT_CRITICAL				= 3;

	static GLboolean check(AESprite* s1, AESprite* s2, Frame* f1, Frame* f2, AEPoint* pSparkPos, AEPoint* pBloodPos);
	static GLvoid handle(AESprite* s1, AESprite* s2, Frame* f1, Frame* f2, AEPoint sparkPos, AEPoint bloodPos);

private:



};