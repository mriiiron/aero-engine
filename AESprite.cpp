#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <GL\glut.h>
#include <math.h>
#include "AEUtility.h"
#include "AEResource.h"
#include "AEObject.h"
#include "AEKeyboard.h"
#include "AEBackground.h"
#include "AEParticleSystem.h"
#include "AEScene.h"
#include "AEPhysics.h"
#include "AESprite.h"

extern AEResourceTable rTable;
extern AEObjectTable oTable;
extern AESpriteTable sTable;
extern AEParticleSystem ptclSys;

extern GLint showCrosshair;

AESprite::AESprite(AEScene* scene, GLint _oid, GLint _team, GLfloat _cx, GLfloat _cy, GLint _action, GLint inverse) {
	oid = _oid;  team = _team;  cx = _cx;  cy = _cy;
	vx = vy = ax = ay = gndSpeed = 0.0;
	frame = time = facing = angle = timeToStiff = keyState = atkJudgeLock = 0;
	onLandform = scene->getBackground()->getLandformIndexBelow(GLint(cx), GLint(cy), &drop);
	hpValue = hpMax = 100;
	for (int i = 0; i < AEKeyboardHandler::INPUT_COUNT; i++) inputState[i] = 0;
	if (inverse) turnOver();
	changeAction(_action);
}

AEPoint AESprite::calcRotatedPoint(GLfloat cx, GLfloat cy, Frame f, GLint angleDeg, GLbyte facing) {
	GLfloat angle = angleDeg * 3.14159 / 180.0;
	GLfloat cosA = cos(angle), sinA = sin(angle);
	AEPoint point;
	if (facing == FACING_RIGHT) {
		point.x = cx + (f.cast->x - f.centerx) * cosA - (f.cast->y - f.centery) * sinA;
		point.y = cy + (f.cast->x - f.centerx) * sinA + (f.cast->y - f.centery) * cosA;
	}
	else {
		point.x = cx - (f.cast->x - f.centerx) * cosA + (f.cast->y - f.centery) * sinA;
		point.y = cy + (f.cast->x - f.centerx) * sinA + (f.cast->y - f.centery) * cosA;
	}
	return point;
}

AEBiasRect AESprite::calcRotatedRect(GLfloat cx, GLfloat cy, Frame f, GLint angleDeg, GLbyte facing) {
	GLfloat angle = angleDeg * 3.14159 / 180.0;
	GLfloat cosA = cos(angle), sinA = sin(angle);
	AEBiasRect bRect;
	if (facing == FACING_RIGHT) {
		bRect.x1 = cx - f.centerx * cosA + f.centery * sinA;
		bRect.y1 = cy - f.centerx * sinA - f.centery * cosA;
		bRect.x2 = bRect.x1 + f.width * cosA;
		bRect.y2 = bRect.y1 + f.width * sinA;
		bRect.x3 = bRect.x2 - f.height * sinA;
		bRect.y3 = bRect.y2 + f.height * cosA;
		bRect.x4 = bRect.x1 - f.height * sinA;
		bRect.y4 = bRect.y1 + f.height * cosA;
	}
	else {
		bRect.x2 = cx + f.centerx * cosA - f.centery * sinA;
		bRect.y2 = cy - f.centerx * sinA - f.centery * cosA;
		bRect.x1 = bRect.x2 - f.width * cosA;
		bRect.y1 = bRect.y2 + f.width * sinA;
		bRect.x3 = bRect.x2 + f.height * sinA;
		bRect.y3 = bRect.y2 + f.height * cosA;
		bRect.x4 = bRect.x1 + f.height * sinA;
		bRect.y4 = bRect.y1 + f.height * cosA;
	}
	return bRect;
}

GLvoid AESprite::changeAction(GLint _action) {
	if (_action == 1000) {
		sTable.remove(index);
		return;
	}
	action = _action;
	Animation anim = oTable.get(oid)->getAnim(action);
	state = anim.getState();
	timeToLive = anim.getTTL();
	frame = time = 0;
	if (state >= AEObject::STATE_CHAR_ACTION) {
		GLint dvx = anim.getFrame(frame).dvx;
		if (dvx == 999)
			vx = 0;
		else
			vx += dvx;
		GLint dvy = anim.getFrame(frame).dvy;
		if (dvy == 999)
			vy = 0;
		else
			vy += dvy;
	}
	else { 
		ax = vx = ay = vy = 0.0;
	}
	if (anim.getFrame(frame).cast != NULL) {
		Frame f = anim.getFrame(frame);
		AEPoint castPoint = calcRotatedPoint(cx, cy, f, angle, facing);
		if (facing == 0)
			sTable.add(new AESprite(f.cast->oid, team, castPoint.x, castPoint.y, f.cast->action));
		else
			sTable.add(new AESprite(f.cast->oid, team, castPoint.x, castPoint.y, f.cast->action, CAST_INVERSE));
	}
}

GLint AESprite::log2(GLint key) {
	switch (key) {
	case 0x1: return 0;
	case 0x2: return 1;
	case 0x4: return 2;
	case 0x8: return 3;
	case 0x10: return 4;
	case 0x20: return 5;
	case 0x40: return 6;
	default: return -1;
	}
}

GLvoid AESprite::pressUp() {
	keyDown(AEKeyboardHandler::AKS_UP);
	inputState[AEKeyboardHandler::INPUT_8] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
}

GLvoid AESprite::pressDown() {
	keyDown(AEKeyboardHandler::AKS_DOWN);
	inputState[AEKeyboardHandler::INPUT_2] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
}

GLvoid AESprite::pressLeft() {
	keyDown(AEKeyboardHandler::AKS_LEFT);
	if (facing == FACING_RIGHT)
		inputState[AEKeyboardHandler::INPUT_4] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
	else
		inputState[AEKeyboardHandler::INPUT_6] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
}

GLvoid AESprite::pressRight() {
	keyDown(AEKeyboardHandler::AKS_RIGHT);
	if (facing == FACING_RIGHT)
		inputState[AEKeyboardHandler::INPUT_6] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
	else
		inputState[AEKeyboardHandler::INPUT_4] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
}

GLvoid AESprite::pressAttack() {
	if (!isKeyDown(AEKeyboardHandler::BKS_ATTACK)) {
		keyDown(AEKeyboardHandler::BKS_ATTACK);
		if (isKeyDown(AEKeyboardHandler::AKS_UP)) {
			inputState[AEKeyboardHandler::INPUT_8A] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
		}
		else if (isKeyDown(AEKeyboardHandler::AKS_DOWN)) {
			inputState[AEKeyboardHandler::INPUT_2A] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
		}
		else if (isKeyDown(AEKeyboardHandler::AKS_RIGHT) && facing == FACING_RIGHT || isKeyDown(AEKeyboardHandler::AKS_LEFT) && facing == FACING_LEFT) {
			inputState[AEKeyboardHandler::INPUT_6A] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
		}
		else if (isKeyDown(AEKeyboardHandler::AKS_RIGHT) && facing == FACING_LEFT || isKeyDown(AEKeyboardHandler::AKS_LEFT) && facing == FACING_RIGHT) {
			inputState[AEKeyboardHandler::INPUT_4A] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
		}
		else {
			inputState[AEKeyboardHandler::INPUT_A] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
		}
	}
}

GLvoid AESprite::pressJump() {
	if (!isKeyDown(AEKeyboardHandler::BKS_JUMP)) {
		keyDown(AEKeyboardHandler::BKS_JUMP);
		inputState[AEKeyboardHandler::INPUT_J] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
	}
}

GLvoid AESprite::pressDefend() {
	if (!isKeyDown(AEKeyboardHandler::BKS_DEFEND)) {
		keyDown(AEKeyboardHandler::BKS_DEFEND);
		if (isKeyDown(AEKeyboardHandler::AKS_UP)) {
			inputState[AEKeyboardHandler::INPUT_8D] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
		}
		else {
			inputState[AEKeyboardHandler::INPUT_D] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
		}
	}
}

GLvoid AESprite::pressForward() {
	if (facing == FACING_RIGHT)
		pressRight();
	else
		pressLeft();
}

GLvoid AESprite::pressBackward() {
	if (facing == FACING_LEFT)
		pressRight();
	else
		pressLeft();
}

GLvoid AESprite::releaseUp() {
	keyUp(AEKeyboardHandler::AKS_UP);
}

GLvoid AESprite::releaseDown() {
	keyUp(AEKeyboardHandler::AKS_DOWN);
}

GLvoid AESprite::releaseLeft() {
	keyUp(AEKeyboardHandler::AKS_LEFT);
	if (facing == FACING_RIGHT && action == AEObject::STATE_CHAR_BACKWARD || facing == FACING_LEFT && action == AEObject::STATE_CHAR_FORWARD) {
		changeAction(AEObject::ACT_CHAR_STAND);
		setGroundSpeed(0.0);
	}
}

GLvoid AESprite::releaseRight() {
	keyUp(AEKeyboardHandler::AKS_RIGHT);
	if (facing == FACING_RIGHT && action == AEObject::STATE_CHAR_FORWARD || facing == FACING_LEFT && action == AEObject::STATE_CHAR_BACKWARD) {
		changeAction(AEObject::ACT_CHAR_STAND);
		setGroundSpeed(0.0);
	}	
}

GLvoid AESprite::releaseAttack() {
	keyUp(AEKeyboardHandler::BKS_ATTACK);
}

GLvoid AESprite::releaseJump() {
	keyUp(AEKeyboardHandler::BKS_JUMP);
}

GLvoid AESprite::releaseDefend() {
	keyUp(AEKeyboardHandler::BKS_DEFEND);
}

GLvoid AESprite::input(GLint _input) {
	inputState[_input] = AEKeyboardHandler::INPUT_JUDGE_TOLERATION;
}

GLbyte AESprite::inputStateJudge(GLint _input) {
	if (inputState[_input] > 0)
		return 1;
	else
		return 0;
}

GLvoid AESprite::testKeyState() {
	if (state >= AEObject::STATE_CHAR_ACTION)
		return;
	if (inputStateJudge(AEKeyboardHandler::INPUT_A)) {
		changeAction(AEObject::ACT_CHAR_ATTACK_H);
	}
	else if (inputStateJudge(AEKeyboardHandler::INPUT_8A)) {
		changeAction(AEObject::ACT_CHAR_ATTACK_V);
	}
	else if (inputStateJudge(AEKeyboardHandler::INPUT_2A)) {
		changeAction(AEObject::ACT_CHAR_ATTACK);
	}
	else if (inputStateJudge(AEKeyboardHandler::INPUT_4A)) {
		changeAction(AEObject::ACT_CHAR_ATTACK_H);
	}
	else if (inputStateJudge(AEKeyboardHandler::INPUT_6A)) {
		changeAction(AEObject::ACT_CHAR_ATTACK_H);
	}
	else if (inputStateJudge(AEKeyboardHandler::INPUT_J)) {
		// Doing nothing
	}
	else if (inputStateJudge(AEKeyboardHandler::INPUT_D)) {
		changeAction(AEObject::ACT_CHAR_DEFEND_V);
	}
	else if (inputStateJudge(AEKeyboardHandler::INPUT_8D)) {
		changeAction(AEObject::ACT_CHAR_DEFEND_H);
	}
	else if (isKeyDown(AEKeyboardHandler::AKS_LEFT)) {
		if (action == AEObject::STATE_CHAR_STAND) {
			if (facing == FACING_RIGHT) {
				changeAction(AEObject::ACT_CHAR_BACKWARD);
				setGroundSpeed(-3.0);
			}
			else {
				changeAction(AEObject::ACT_CHAR_FORWARD);
				setGroundSpeed(3.0);
			}
		}
	}
	else if (isKeyDown(AEKeyboardHandler::AKS_RIGHT)) {
		if (action == AEObject::STATE_CHAR_STAND) {
			if (facing == FACING_RIGHT) {
				changeAction(AEObject::ACT_CHAR_FORWARD);
				setGroundSpeed(3.0);
			}
			else {
				changeAction(AEObject::ACT_CHAR_BACKWARD);
				setGroundSpeed(-3.0);
			}
		}
	}
}

GLvoid AESprite::update() {
	Background* bg = bTable.getActiveBG();
	for (int i = 0; i < AEKeyboardHandler::INPUT_COUNT; i++) {
		if (inputState[i] > 0)
			inputState[i]--;
	}
	if (timeToStiff > 0) {
		timeToStiff--;
		return;
	}
	Animation anim = oTable.get(oid)->getAnim(action);
	if (timeToLive == 0) {
		changeAction(anim.getNext());
		return;
	}
	if (timeToLive > 0) timeToLive--;
	GLint fac;
	if (facing) fac = -1; else fac = 1;
	cx += (fac * vx);
	cy += vy;
	vx += ax;
	vy += ay;
	angle = angle % 360;
	if (state < 1000) {
		if (state >= AEObject::STATE_CHAR_INAIR) {
			if (drop + vy <= 0) {
				drop = 0;
				cy = bg->getLocation().y + bg->getLandform(onLandform).data[bg->getXonBG(GLint(cx))].altitude;
				if (state == AEObject::STATE_CHAR_INAIR) {
					changeAction(AEObject::ACT_CHAR_LANDING);
					vy = ay = 0.0;
				}
				else if (state == AEObject::STATE_CHAR_FALLING) {
					if (abs(vy) >= AEPhysics::CHAR_REBOUND_SPEED) {
						changeAction(AEObject::ACT_CHAR_FALL_REBOUND);
						vy = 2.0;
						ay = AEPhysics::GRAVITY;
					}
					else {
						changeAction(AEObject::ACT_CHAR_LYING);
						vy = ay = 0.0;
					}
				}
				else if (state == AEObject::STATE_CHAR_FALLING_R) {
					changeAction(AEObject::ACT_CHAR_LYING);
					vy = ay = 0.0;
				}
			}
			else {
				onLandform = bg->getLandformIndexBelow(GLint(cx), GLint(cy), &drop);
				if (state == AEObject::STATE_CHAR_FALLING) {
					if (abs(vx) > 0.1) {
						GLfloat slope = -vy / vx;
						if (slope > tan(75 * 3.14159 / 180)) {
							changeAction(AEObject::ACT_CHAR_FALL_90);
						}
						else if (slope > tan(45 * 3.14159 / 180)) {
							changeAction(AEObject::ACT_CHAR_FALL_60);
						}
						else if (slope > tan(15 * 3.14159 / 180)) {
							changeAction(AEObject::ACT_CHAR_FALL_30);
						}
						else if (slope > tan(-15 * 3.14159 / 180)) {
							changeAction(AEObject::ACT_CHAR_FALL_0);
						}
						else {
							changeAction(AEObject::ACT_CHAR_FALL_DROP);
						}
					}
					else {
						if (vy > 0.373) {
							changeAction(AEObject::ACT_CHAR_FALL_90);
						}
						else if (vy > 0.100) {
							changeAction(AEObject::ACT_CHAR_FALL_60);
						}
						else if (vy > 0.027) {
							changeAction(AEObject::ACT_CHAR_FALL_30);
						}
						else if (vy > -0.027) {
							changeAction(AEObject::ACT_CHAR_FALL_0);
						}
						else {
							changeAction(AEObject::ACT_CHAR_FALL_DROP);
						}
					}
					ay = AEPhysics::GRAVITY;
				}
			}
		}
		else {
			GLint lastLandform = onLandform;
			onLandform = bg->getLandformIndexBelow(GLint(cx), GLint(cy), &drop);
			if (abs(drop) <= AEBackground::ONLANDFORM_TOLERANCE) {
				drop = 0;
				cy = bg->getLocation().y + bg->getLandform(onLandform).data[bg->getXonBG(GLint(cx))].altitude;
				if (state >= AEObject::STATE_CHAR_ACTION) {
					if (gndSpeed != 0.0) {
						ax = AEPhysics::RESISTANCE_GROUND;
						if (gndSpeed * ax > 0) ax = -ax;
						GLfloat newSpeed = gndSpeed + ax;
						if (newSpeed * gndSpeed <= 0) {
							ax = newSpeed = 0.0;
						}
						gndSpeed = newSpeed;
					}
				}
				GLfloat slope = bg->getLandform(onLandform).data[bg->getXonBG(GLint(cx))].slope;
				GLfloat string = sqrt(1.0 + slope * slope);
				vx = gndSpeed * 1.0 / string;
				vy = gndSpeed * fac * slope / string;
			}
			else {
				changeAction(AEObject::ACT_CHAR_INAIR);
				ay = AEPhysics::GRAVITY;
			}
		}
	}
	JumpParas* jump = anim.getFrame(frame).jumpTo;
	KeyReleaseParas* release = anim.getFrame(frame).keyRelease;
	if (jump != NULL) {
		for (GLint input = 0; input < AEKeyboardHandler::INPUT_COUNT; input++) {
			if (jump->action[input] > 0 && inputStateJudge(input)) {
				changeAction(jump->action[input]);
				return;
			}
		}
	}
	if (release != NULL && (!isKeyDown(release->key))) {
		changeAction(release->action);
		return;
	}
	time++;
	if (time >= anim.getEndTime(frame)) {
		unlockAtkJudge();
		cx += (fac * anim.getFrame(frame).shiftx);
		cy += anim.getFrame(frame).shifty;
		if (state <= AEObject::STATE_CHAR_INAIR) {
			cy = bg->getLocation().y + bg->getLandform(onLandform).data[bg->getXonBG(GLint(cx))].altitude;
		}
		angle += anim.getFrame(frame).rotate;
		HoldParas* hold = anim.getFrame(frame).hold;
		if (hold != NULL && isKeyDown(hold->key) > 0) {
			if (frame == 0)
				time = 0;
			else
				time = anim.getEndTime(frame - 1);
		}	
		else {
			frame++;
			if (time >= anim.getEndTime(anim.getFrameCount() - 1)) {
				time = 0;
			}
			if (frame == anim.getFrameCount()) {
				frame = 0;
				if (!anim.isLoop()) {
					changeAction(anim.getNext());
					return;
				}
			}
			if (state >= AEObject::STATE_CHAR_ACTION) {
				if (state < AEObject::STATE_CHAR_INAIR) {
					GLint dvx = anim.getFrame(frame).dvx;
					if (dvx == 999)
						gndSpeed = 0;
					else
						gndSpeed += dvx;
				}
				else {
					GLint dvx = anim.getFrame(frame).dvx;
					if (dvx == 999)
						vx = 0;
					else
						vx += dvx;
				}
			}
			if (anim.getFrame(frame).cast != NULL) {
				Frame f = anim.getFrame(frame);
				AEPoint castPoint = calcRotatedPoint(cx, cy, f, angle, facing);
				if (facing == 0)
					sTable.add(new AESprite(f.cast->oid, team, castPoint.x, castPoint.y, f.cast->action));
				else
					sTable.add(new AESprite(f.cast->oid, team, castPoint.x, castPoint.y, f.cast->action, CAST_INVERSE));
			}
		}
	}
	testKeyState();
}

GLvoid AESprite::paintShadow() {
	AEResource* shadow = rTable.get(0);
	AERect shadowRect = createRect(cx - shadow->getCellWidth() / 2, cy - drop - shadow->getCellHeight() / 2, cx + shadow->getCellWidth() / 2, cy - drop + shadow->getCellHeight() / 2);
	paintRect(shadow->getTexture(), createRect(0.0, 0.0, 1.0, 1.0), shadowRect);
}

GLvoid AESprite::paintCrosshair() {
	AEResource* crosshair = rTable.get(100);
	AERect crossRect = createRect(cx - crosshair->getCellWidth() / 2, cy - crosshair->getCellHeight() / 2, cx + crosshair->getCellWidth() / 2, cy + crosshair->getCellHeight() / 2);
	paintRect(crosshair->getTexture(), createRect(0.0, 0.0, 1.0, 1.0), crossRect);
}

GLvoid AESprite::paint() {
	glColor3f(1.0, 1.0, 1.0);
	AEObject* obj = oTable.get(oid);
	if (obj->getType() < 1)
		paintShadow();
	Frame f = obj->getAnim(action).getFrame(frame);
	AEResource* res = rTable.get(f.rid);
	AERect texClip = res->getTexCoords(f.imgOffset, f.imgCells);
	AEBiasRect sprRect = calcRotatedRect(cx, cy, f, angle, facing);
	if (facing == FACING_RIGHT)
		paintRect(res->getTexture(), texClip, sprRect);
	else
		paintRect(res->getTexture(), getInversedRect(texClip, INVERSE_X), sprRect);
	if (showCrosshair)
		paintCrosshair();
}

char* AESprite::getObjName() {
	return oTable.get(oid)->getName();
}

AESpriteTable::AESpriteTable() {
	maxIndex = -1;
	pHash = 0;
	for (int i = 0; i < MAX_ONLINE_SPRITES; i++) {
		occupied[i] = 0;
		hash[i] = -1;
	}
}

GLvoid AESpriteTable::add(AESprite* sp) {
	for (GLint i = 0; i <= maxIndex; i++) {
		if (!occupied[i]) {
			sp->setIndex(i);
			table[i] = sp;  occupied[i] = 1;
			hash[pHash] = i;  pHash++;
			return;
		}
	}
	if (maxIndex == MAX_ONLINE_SPRITES - 1) {
		printf("Too many sprites online ..\n");
	}
	else {
		maxIndex++;
		sp->setIndex(maxIndex);
		table[maxIndex] = sp;  occupied[maxIndex] = 1;
		hash[pHash] = maxIndex;  pHash++;
	}
}

GLvoid AESpriteTable::addAt(GLint index, AESprite* sp) {
	if (occupied[index]) {
		printf("Error on adding sprite: Slot Occupied.\n");
		return;
	}
	if (index > maxIndex) {
		maxIndex = index;
	}
	sp->setIndex(index);
	table[index] = sp;  occupied[index] = 1;
	hash[pHash] = maxIndex;  pHash++;
}

GLvoid AESpriteTable::remove(GLint index) {
	if (!occupied[index]) {
		printf("Error on removing sprite: Slot Empty.\n");
		return;
	}
	delete table[index];
	table[index] = NULL;
	occupied[index] = 0;
	while (!occupied[maxIndex] && maxIndex > 0) maxIndex--;
	for (int i = 0; i < pHash; i++) {
		if (hash[i] == index) {
			pHash--;
			hash[i] = hash[pHash];
			hash[pHash] = -1;
			break;
		}
	}
}

GLvoid AESpriteTable::clear() {
	for (int i = 0; i <= maxIndex; i++) {
		if (occupied[i])
			remove(i);
	}
}

GLvoid AESpriteTable::singleCollision(AESprite* s1, AESprite* s2, Frame f1, Frame f2, AEPoint sparkPos, AEPoint bloodPos) {
	printf("Collision: Sprite %d got collided by Sprite %d .. ", s2->getIndex(), s1->getIndex());
	switch (s1->getState()) {
	case 1000:
		s1->changeAction(AEObject::ACT_PROJ_HIT);
		break;
	default:
		break;
	}
	if (f2.block != NULL) {
		if (f1.attack->type == 99) {
			if (f2.block->type == 99) {
				printf("Clash!\n");
				sTable.add(new AESprite(OBJECT_SPARK, AESprite::TEAM_NONE, sparkPos.x, sparkPos.y, SPARK_SPECIAL));
				s1->changeAction(f1.attack->blownOffTo);
				s2->changeAction(f2.block->breakTo);
				s1->stiffen(4);
				s2->stiffen(4);
				if (s1->getState() >= AEObject::STATE_CHAR_INAIR)
					s1->setVx(-f1.attack->force / 2.0);
				else
					s1->setGroundSpeed(-f1.attack->force / 2.0);
				if (s2->getState() >= AEObject::STATE_CHAR_INAIR)
					s2->setVx(-f1.attack->force / 2.0);
				else
					s2->setGroundSpeed(-f1.attack->force / 2.0);
			}
			else {
				printf("Blocked, but cannot force back!\n");
				sTable.add(new AESprite(OBJECT_SPARK, AESprite::TEAM_NONE, sparkPos.x, sparkPos.y, SPARK_BLOCK));
				s2->changeAction(f2.block->blockTo);
				s1->stiffen(3);
				s2->stiffen(3);
				if (s2->getState() >= AEObject::STATE_CHAR_INAIR)
					s2->setVx(-f1.attack->force);
				else
					s2->setGroundSpeed(-f1.attack->force);
			}
		}
		else if (f2.block->type == 99) {
			printf("Blown off!\n");
			sTable.add(new AESprite(OBJECT_SPARK, AESprite::TEAM_NONE, sparkPos.x, sparkPos.y, SPARK_SPECIAL));
			s1->changeAction(f1.attack->blownOffTo);
			s1->stiffen(5);
			s2->stiffen(2);
			if (s1->getState() >= STATE_CHAR_INAIR)
				s1->setVx(-f1.attack->force / 2.0);
			else
				s1->setGroundSpeed(-f1.attack->force / 2.0);
		}
		else if (f1.attack->type == 0 || f2.block->type == 0 || f1.attack->type == f2.block->type) {
			printf("Blocked.\n");
			sTable.add(new AESprite(OBJECT_SPARK, AESprite::TEAM_NONE, sparkPos.x, sparkPos.y, SPARK_BLOCK));
			s1->changeAction(f1.attack->blockedTo);
			s2->changeAction(f2.block->blockTo);
			s2->takeDamage(f1.attack->damage / 10);
			s2->stiffen(3);
			if (s1->getState() >= AEObject::STATE_CHAR_INAIR)
				s1->setVx(-f1.attack->force / 2.0);
			else
				s1->setGroundSpeed(-f1.attack->force / 2.0);
			if (s2->getState() >= AEObject::STATE_CHAR_INAIR)
				s2->setVx(-f1.attack->force / 1.5);
			else
				s2->setGroundSpeed(-f1.attack->force / 1.5);
		}
		else {
			printf("Hit! Guard failed.\n");
			GLint sparkType;
			switch (f1.attack->effect) {
			case 1:
				sparkType = SPARK_SLASH_HEAVY;
				break;
			default:
				sparkType = SPARK_HIT;
				break;
			}
			AESprite* spark = new AESprite(OBJECT_SPARK, AESprite::TEAM_NONE, sparkPos.x, sparkPos.y, sparkType);
			spark->setAngle(f1.attack->angle);
			spark->setFacing(s1->getFacing());
			sTable.add(spark);
			s2->changeAction(AEObject::ACT_CHAR_GET_HIT);
			s2->takeDamage(f1.attack->damage);
			if (s2->getHP() <= 0) {
				sTable.add(new AESprite(OBJECT_SPARK, AESprite::TEAM_NONE, bloodPos.x, bloodPos.y, BLOOD_CRITICAL));
			}
			s1->stiffen(4);
			s2->stiffen(4);
			if (f1.attack->force >= 10) {
				if (f1.attack->angle > 75) {
					s2->changeAction(AEObject::ACT_CHAR_FALL_90);
				}
				else if (f1.attack->angle > 45) {
					s2->changeAction(AEObject::ACT_CHAR_FALL_60);
				}
				else if (f1.attack->angle > 15) {
					s2->changeAction(AEObject::ACT_CHAR_FALL_30);
				}
				else if (f1.attack->angle > -15) {
					s2->changeAction(AEObject::ACT_CHAR_FALL_0);
				}
				else {
					s2->changeAction(AEObject::ACT_CHAR_FALL_DROP);
				}
				s2->setGroundSpeed(0.0);
				s2->setAx(0.0);
				s2->setVx(-f1.attack->force * cos(f1.attack->angle * 3.14159 / 180));
				s2->setVy(f1.attack->force * sin(f1.attack->angle * 3.14159 / 180));
			}
			else {
				s2->changeAction(AEObject::ACT_CHAR_GET_HIT);
				if (s2->getState() >= AEObject::STATE_CHAR_INAIR)
					s2->setVx(-f1.attack->force);
				else
					s2->setGroundSpeed(-f1.attack->force);
			}
		}
	}
	else {
		printf("Hit.\n");
		GLint sparkType;
		switch (f1.attack->effect) {
		case 1:
			sparkType = SPARK_SLASH_HEAVY;
			break;
		default:
			sparkType = SPARK_HIT;
			break;
		}
		AESprite* spark = new AESprite(OBJECT_SPARK, AESprite::TEAM_NONE, sparkPos.x, sparkPos.y, sparkType);
		spark->setAngle(f1.attack->angle);
		spark->setFacing(s1->getFacing());
		sTable.add(spark);
		s2->takeDamage(f1.attack->damage);
		s1->stiffen(4);
		s2->stiffen(4);
		if (f1.attack->force >= 10) {
			if (f1.attack->angle > 75) {
				s2->changeAction(AEObject::ACT_CHAR_FALL_90);
			}
			else if (f1.attack->angle > 45) {
				s2->changeAction(AEObject::ACT_CHAR_FALL_60);
			}
			else if (f1.attack->angle > 15) {
				s2->changeAction(AEObject::ACT_CHAR_FALL_30);
			}
			else if (f1.attack->angle > -15) {
				s2->changeAction(AEObject::ACT_CHAR_FALL_0);
			}
			else {
				s2->changeAction(AEObject::ACT_CHAR_FALL_DROP);
			}
			s2->setGroundSpeed(0.0);
			s2->setAx(0.0);
			s2->setVx(-f1.attack->force * cos(f1.attack->angle * 3.14159 / 180));
			s2->setVy(f1.attack->force * sin(f1.attack->angle * 3.14159 / 180));
		}
		else {
			s2->changeAction(AEObject::ACT_CHAR_GET_HIT);
			if (s2->getState() >= AEObject::STATE_CHAR_INAIR)
				s2->setVx(-f1.attack->force);
			else
				s2->setGroundSpeed(-f1.attack->force);
		}
	}
	if (s2->getHP() <= 0) {
		sTable.add(new AESprite(OBJECT_SPARK, AESprite::TEAM_NONE, bloodPos.x, bloodPos.y, BLOOD_CRITICAL));
		printf("KO.\n");
		s2->changeAction(AEObject::ACT_CHAR_DEAD);
	}
	s1->lockAtkJudge();
}

GLvoid AESpriteTable::dealWithCollisions() {
	for (GLint i = 0; i < pHash - 1; i++) {
		AESprite* s1 = table[hash[i]];
		Frame f1 = oTable.get(s1->getOid())->getAnim(s1->getAction()).getFrame(s1->getFrame());
		AEPoint sparkPos, bloodPos;
		if (!(s1->isAtkJudgeLocked()) && f1.attack != NULL) {
			for (GLint j = i + 1; j < pHash; j++) {
				AESprite* s2 = table[hash[j]];
				if (s1->getTeam() == s2->getTeam())
					continue;
				Frame f2 = oTable.get(s2->getOid())->getAnim(s2->getAction()).getFrame(s2->getFrame());
				if (f2.body != NULL && isCollided(s1, s2, f1, f2, &sparkPos, &bloodPos)) {
					singleCollision(s1, s2, f1, f2, sparkPos, bloodPos);
				}
			}
		}
		if (f1.body != NULL) {
			for (GLint j = i + 1; j < pHash; j++) {
				AESprite* s2 = table[hash[j]];
				if (s2->isAtkJudgeLocked() || s1->getTeam() == s2->getTeam())
					continue;
				Frame f2 = oTable.get(s2->getOid())->getAnim(s2->getAction()).getFrame(s2->getFrame());
				if (f2.attack != NULL && isCollided(s2, s1, f2, f1, &sparkPos, &bloodPos)) {
					singleCollision(s2, s1, f2, f1, sparkPos, bloodPos);
				}
			}
		}
	}
}

GLvoid AESpriteTable::update() {
	for (int i = 0; i <= maxIndex; i++) {
		if (occupied[i])
			table[i]->update();
	}
	dealWithCollisions();
}

GLvoid AESpriteTable::paint() {
	for (int i = 0; i <= maxIndex; i++) {
		if (occupied[i])
			table[i]->paint();
	}
}