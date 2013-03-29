#include <stdio.h>
#include <GL\glut.h>
#include <math.h>
#include "AEUtility.h"
#include "AEObject.h"
#include "AESprite.h"
#include "AECollision.h"

AECollisionResult AECollision::check(AESprite* s1, AESprite* s2, Frame* f1, Frame* f2) {
	GLint s1x1, s1y1, s1x2, s1y2, s2x1, s2y1, s2x2, s2y2;
	GLint cx1 = s1->getCx(), cy1 = s1->getCy(), centerx1 = f1->centerx, centery1 = f1->centery;
	GLint cx2 = s2->getCx(), cy2 = s2->getCy(), centerx2 = f2->centerx, centery2 = f2->centery;
	AtkParas* r1 = f1->attack;  BdyParas* r2 = f2->body;
	if (s1->getFacing() == AESprite::FACING_RIGHT) {
		s1x1 = cx1 - centerx1 + r1->x1;  s1x2 = cx1 - centerx1 + r1->x2;
	}
	else {
		s1x1 = cx1 + centerx1 - r1->x2;  s1x2 = cx1 + centerx1 - r1->x1;
	}
	s1y1 = cy1 - centery1 + r1->y1;  s1y2 = cy1 - centery1 + r1->y2;
	if (s2->getFacing() == AESprite::FACING_RIGHT) {
		s2x1 = cx2 - centerx2 + r2->x1;  s2x2 = cx2 - centerx2 + r2->x2;
	}
	else {
		s2x1 = cx2 + centerx2 - r2->x2;  s2x2 = cx2 + centerx2 - r2->x1;
	}
	s2y1 = cy2 - centery2 + r2->y1;  s2y2 = cy2 - centery2 + r2->y2;
	if (s1x1 < s2x2 && s1y1 < s2y2 && s1x2 > s2x1 && s1y2 > s2y1) {
		GLfloat x = (s1x1 + s1x2 + s2x1 + s2x2) / 4.0f;
		GLfloat y = (s1y1 + s1y2 + s2y1 + s2y2) / 4.0f;
		return AECollisionResult(AECollisionResult::TYPE_NORMAL, AEUtil::createPoint(x, y));
	}
	else {
		return AECollisionResult(AECollisionResult::TYPE_NONE);
	}
}

static AECollisionResult ray(AESprite* s1, AESprite* s2, Frame* f1) {


}

GLvoid AECollision::handle(AESprite* s1, AESprite* s2, Frame* f1, Frame* f2, AEPoint sparkPos) {
	printf("Collision: Sprite %d got collided by Sprite %d .. ", s2->getIndex(), s1->getIndex());
	switch (s1->getState()) {
	case 1000:
		s1->changeAction(AEObject::ACT_PROJ_HIT);
		break;
	default:
		break;
	}
	if (f2->block != NULL) {
		if (f1->attack->type == 99) {
			if (f2->block->type == 99) {
				printf("Clash!\n");
				// sTable.add(new AESprite(s1->getScene(), OBJECT_SPARK, AESprite::TEAM_NONE, sparkPos.x, sparkPos.y, SPARK_SPECIAL));
				s1->changeAction(f1->attack->blownOffTo);
				s2->changeAction(f2->block->breakTo);
				s1->stiffen(4);
				s2->stiffen(4);
				if (s1->getState() >= AEObject::STATE_CHAR_INAIR)
					s1->setVx(-f1->attack->force / 2.0);
				else
					s1->setGroundSpeed(-f1->attack->force / 2.0);
				if (s2->getState() >= AEObject::STATE_CHAR_INAIR)
					s2->setVx(-f1->attack->force / 2.0);
				else
					s2->setGroundSpeed(-f1->attack->force / 2.0);
			}
			else {
				printf("Blocked, but cannot force back!\n");
				// sTable.add(new AESprite(s1->getScene(), OBJECT_SPARK, AESprite::TEAM_NONE, sparkPos.x, sparkPos.y, SPARK_BLOCK));
				s2->changeAction(f2->block->blockTo);
				s1->stiffen(3);
				s2->stiffen(3);
				if (s2->getState() >= AEObject::STATE_CHAR_INAIR)
					s2->setVx(-f1->attack->force);
				else
					s2->setGroundSpeed(-f1->attack->force);
			}
		}
		else if (f2->block->type == 99) {
			printf("Blown off!\n");
			// sTable.add(new AESprite(s1->getScene(), OBJECT_SPARK, AESprite::TEAM_NONE, sparkPos.x, sparkPos.y, SPARK_SPECIAL));
			s1->changeAction(f1->attack->blownOffTo);
			s1->stiffen(5);
			s2->stiffen(2);
			if (s1->getState() >= AEObject::STATE_CHAR_INAIR)
				s1->setVx(-f1->attack->force / 2.0);
			else
				s1->setGroundSpeed(-f1->attack->force / 2.0);
		}
		else if (f1->attack->type == 0 || f2->block->type == 0 || f1->attack->type == f2->block->type) {
			printf("Blocked.\n");
			// sTable.add(new AESprite(s1->getScene(), OBJECT_SPARK, AESprite::TEAM_NONE, sparkPos.x, sparkPos.y, SPARK_BLOCK));
			s1->changeAction(f1->attack->blockedTo);
			s2->changeAction(f2->block->blockTo);
			s2->takeDamage(f1->attack->damage / 10);
			s2->stiffen(3);
			if (s1->getState() >= AEObject::STATE_CHAR_INAIR)
				s1->setVx(-f1->attack->force / 2.0);
			else
				s1->setGroundSpeed(-f1->attack->force / 2.0);
			if (s2->getState() >= AEObject::STATE_CHAR_INAIR)
				s2->setVx(-f1->attack->force / 1.5);
			else
				s2->setGroundSpeed(-f1->attack->force / 1.5);
		}
		else {
			printf("Hit! Guard failed.\n");
			GLint sparkType;
			switch (f1->attack->effect) {
			case 1:
				sparkType = SPARK_SLASH_HEAVY;
				break;
			default:
				sparkType = SPARK_HIT;
				break;
			}
			// AESprite* spark = new AESprite(s1->getScene(), OBJECT_SPARK, AESprite::TEAM_NONE, sparkPos.x, sparkPos.y, sparkType);
			// spark->setAngle(f1->attack->angle);
			// spark->setFacing(s1->getFacing());
			// sTable.add(spark);
			s2->changeAction(AEObject::ACT_CHAR_GET_HIT);
			s2->takeDamage(f1->attack->damage);
			if (s2->getHP() <= 0) {
				// sTable.add(new AESprite(s1->getScene(), OBJECT_SPARK, AESprite::TEAM_NONE, bloodPos.x, bloodPos.y, BLOOD_CRITICAL));
			}
			s1->stiffen(4);
			s2->stiffen(4);
			if (f1->attack->force >= 10) {
				if (f1->attack->angle > 75) {
					s2->changeAction(AEObject::ACT_CHAR_FALL_90);
				}
				else if (f1->attack->angle > 45) {
					s2->changeAction(AEObject::ACT_CHAR_FALL_60);
				}
				else if (f1->attack->angle > 15) {
					s2->changeAction(AEObject::ACT_CHAR_FALL_30);
				}
				else if (f1->attack->angle > -15) {
					s2->changeAction(AEObject::ACT_CHAR_FALL_0);
				}
				else {
					s2->changeAction(AEObject::ACT_CHAR_FALL_DROP);
				}
				s2->setGroundSpeed(0.0);
				s2->setAx(0.0);
				s2->setVx(-f1->attack->force * cos(f1->attack->angle * 3.14159 / 180));
				s2->setVy(f1->attack->force * sin(f1->attack->angle * 3.14159 / 180));
			}
			else {
				s2->changeAction(AEObject::ACT_CHAR_GET_HIT);
				if (s2->getState() >= AEObject::STATE_CHAR_INAIR)
					s2->setVx(-f1->attack->force);
				else
					s2->setGroundSpeed(-f1->attack->force);
			}
		}
	}
	else {
		printf("Hit.\n");
		GLint sparkType;
		switch (f1->attack->effect) {
		case 1:
			sparkType = SPARK_SLASH_HEAVY;
			break;
		default:
			sparkType = SPARK_HIT;
			break;
		}
		// AESprite* spark = new AESprite(s1->getScene(), OBJECT_SPARK, AESprite::TEAM_NONE, sparkPos.x, sparkPos.y, sparkType);
		// spark->setAngle(f1->attack->angle);
		// spark->setFacing(s1->getFacing());
		// sTable.add(spark);
		s2->takeDamage(f1->attack->damage);
		s1->stiffen(4);
		s2->stiffen(4);
		if (f1->attack->force >= 10) {
			if (f1->attack->angle > 75) {
				s2->changeAction(AEObject::ACT_CHAR_FALL_90);
			}
			else if (f1->attack->angle > 45) {
				s2->changeAction(AEObject::ACT_CHAR_FALL_60);
			}
			else if (f1->attack->angle > 15) {
				s2->changeAction(AEObject::ACT_CHAR_FALL_30);
			}
			else if (f1->attack->angle > -15) {
				s2->changeAction(AEObject::ACT_CHAR_FALL_0);
			}
			else {
				s2->changeAction(AEObject::ACT_CHAR_FALL_DROP);
			}
			s2->setGroundSpeed(0.0);
			s2->setAx(0.0);
			s2->setVx(-f1->attack->force * cos(f1->attack->angle * 3.14159 / 180));
			s2->setVy(f1->attack->force * sin(f1->attack->angle * 3.14159 / 180));
		}
		else {
			s2->changeAction(AEObject::ACT_CHAR_GET_HIT);
			if (s2->getState() >= AEObject::STATE_CHAR_INAIR)
				s2->setVx(-f1->attack->force);
			else
				s2->setGroundSpeed(-f1->attack->force);
		}
	}
	if (s2->getHP() <= 0) {
		// sTable.add(new AESprite(s1->getScene(), OBJECT_SPARK, AESprite::TEAM_NONE, bloodPos.x, bloodPos.y, BLOOD_CRITICAL));
		printf("KO.\n");
		s2->changeAction(AEObject::ACT_CHAR_DEAD);
	}
	s1->lockAtkJudge();
}