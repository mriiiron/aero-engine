#include <GL\glut.h>
#include "AEUtility.h"
#include "AEObject.h"
#include "AESprite.h"
#include "AECollision.h"

GLboolean AECollision::check(AESprite* s1, AESprite* s2, Frame f1, Frame f2, AEPoint* pSparkPos, AEPoint* pBloodPos) {
	GLint s1x1, s1y1, s1x2, s1y2, s2x1, s2y1, s2x2, s2y2;
	GLint cx1 = s1->getCx();  GLint cy1 = s1->getCy();  GLint centerx1 = f1.centerx;  GLint centery1 = f1.centery;
	GLint cx2 = s2->getCx();  GLint cy2 = s2->getCy();  GLint centerx2 = f2.centerx;  GLint centery2 = f2.centery;
	AtkParas* r1 = f1.attack;  BdyParas* r2 = f2.body;
	if (s1->getFacing() == AESprite::FACING_RIGHT) {
		s1x1 = cx1 - centerx1 + r1->x1;  s1x2 = cx1 - centerx1 + r1->x2;
	}
	else {
		s1x1 = cx1 + centerx1 - r1->x2;  s1x2 = cx1 + centerx1 - r1->x1;
	}
	if (s2->getFacing() == AESprite::FACING_RIGHT) {
		s2x1 = cx2 - centerx2 + r2->x1;  s2x2 = cx2 - centerx2 + r2->x2;
	}
	else {
		s2x1 = cx2 + centerx2 - r2->x2;  s2x2 = cx2 + centerx2 - r2->x1;
	}
	s1y1 = cy1 - centery1 + r1->y1;  s1y2 = cy1 - centery1 + r1->y2;
	s2y1 = cy2 - centery2 + r2->y1;  s2y2 = cy2 - centery2 + r2->y2;
	if (s1x1 < s2x2 && s1y1 < s2y2 && s1x2 > s2x1 && s1y2 > s2y1) {
		pSparkPos->x = (s1x1 + s1x2 + s2x1 + s2x2) / 4.0;
		pSparkPos->y = (s1y1 + s1y2 + s2y1 + s2y2) / 4.0;
		pBloodPos->x = (s2x1 + s2x2) / 2.0;
		pBloodPos->y = (s2y1 + s2y2) / 2.0;
		return GL_TRUE;
	}
	else {
		pSparkPos = NULL;
		pBloodPos = NULL;
		return GL_FALSE;
	}
}