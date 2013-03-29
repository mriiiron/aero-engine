#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>
#include <GL\glut.h>
#include "AEUtility.h"
#include "AEObject.h"
#include "AESprite.h"
#include "AEKeyboard.h"
#include "AEAI.h"

extern AEObjectTable oTable;

AEAI::AEAI(AESprite* _self) {
	self = _self;
	target = NULL;
	enabled = 0;
}

GLvoid AEAI::releaseAllKeys() {
	self->resetKeys();
}

//GLvoid AEAI::executeSample() {
//
//	GLint distX;
//
//	// Common AEAI Operations
//	releaseAllKeys();
//	if (self->getState() == AEObject::STATE_CHAR_DEAD) {
//		setTarget(NULL);
//		disable();
//		return;
//	}
//	if (target->getHP() <= 0) {
//		setTarget(NULL);
//		disable();
//		return;
//	}
//
//	switch (type) {
//
//	case TYPE_SWORDMAN:
//
//		for (GLint i = 0; i < sTable.getHashCount(); i++) {
//			AESprite* s = sTable.getByHash(i);
//			if (oTable.get(s->getOid())->getType() == OBJ_PROJECTILE) {
//				if (floor(checkdistXWith(s) / s->getVx()) < 15 && s->getState() == AEObject::STATE_PROJ_FLY) {
//					self->pressDefend();
//					return;
//				}
//			}
//		}
//		distX = checkdistXWith(target);
//		switch (target->getState()) {
//		case AEObject::STATE_CHAR_ATK_H:
//			if (distX < 100) {
//				self->pressDefend();
//			}
//			else if (distX < 120) {
//				GLint alt = AEUtil::randomIntBetween(0, 3);
//				if (alt < 3) {
//					self->pressDefend();
//				}
//				else {
//					self->input(AEKeyboardHandler::INPUT_8A);
//				}
//			}
//			break;
//		case AEObject::STATE_CHAR_ATK_V:
//			if (distX < 150) {
//				self->pressUp();
//				self->pressDefend();
//			}
//			else {
//				self->pressForward();
//			}
//			break;
//		case AEObject::STATE_CHAR_ATK:
//			if (distX < 90) {
//				self->pressDefend();
//			}
//			else if (distX < 120) {
//				self->input(AEKeyboardHandler::INPUT_A);
//			}
//			else { 
//				GLint alt = AEUtil::randomIntBetween(0, 5);
//				if (alt < 1) {
//					self->pressForward();
//				}
//				else if (alt < 3) {
//					self->input(AEKeyboardHandler::INPUT_A);
//				}
//				else if (alt < 5) {
//					self->input(AEKeyboardHandler::INPUT_2A);
//				}
//				else {
//					self->input(AEKeyboardHandler::INPUT_8A);
//				}
//			}
//			break;
//		default:
//			if (distX < 70) {
//				GLint alt = AEUtil::randomIntBetween(0, 3);
//				if (alt < 2) {
//					self->pressBackward();
//				}
//				else {
//					self->input(AEKeyboardHandler::INPUT_A);
//				}
//			}
//			if (distX < 90) {
//				GLint alt = AEUtil::randomIntBetween(0, 3);
//				if (alt < 2) {
//					self->input(AEKeyboardHandler::INPUT_2A);
//				}
//				else {
//					self->input(AEKeyboardHandler::INPUT_A);
//				}
//			}
//			else if (distX < 120) {
//				GLint alt = AEUtil::randomIntBetween(0, 4);
//				if (alt < 2) {
//					self->input(AEKeyboardHandler::INPUT_8A);
//				}
//				else if (alt < 4) {
//					self->input(AEKeyboardHandler::INPUT_2A);
//				}
//				else if (alt < 5) {
//					self->input(AEKeyboardHandler::INPUT_A);
//				}
//				else {
//					self->pressForward();
//				}
//			}
//			else {
//				self->pressForward();
//			}
//		}
//		break;
//
//	case TYPE_GUARD:
//
//		for (GLint i = 0; i < sTable.getHashCount(); i++) {
//			AESprite* s = sTable.getByHash(i);
//			if (oTable.get(s->getOid())->getType() == OBJ_PROJECTILE) {
//				if (floor(checkdistXWith(s) / s->getVx()) < 15 && s->getState() == AEObject::STATE_PROJ_FLY) {
//					self->pressDefend();
//					return;
//				}
//			}
//		}
//		distX = checkdistXWith(target);
//		switch (target->getState()) {
//		case AEObject::STATE_CHAR_ATK_H:
//			if (distX < 100) {
//				self->pressDefend();
//			}
//			else if (distX < 120) {
//				GLint alt = AEUtil::randomIntBetween(0, 3);
//				if (alt < 3) {
//					self->pressDefend();
//				}
//				else {
//					self->input(AEKeyboardHandler::INPUT_2A);
//				}
//			}
//			else if (distX < 140) {
//				GLint alt = AEUtil::randomIntBetween(0, 3);
//				if (alt < 1) {
//					self->input(AEKeyboardHandler::INPUT_2A);
//				}
//				else {
//					self->input(AEKeyboardHandler::INPUT_8A);
//				}
//			}
//			else {
//				self->pressForward();
//			}
//			break;
//		case AEObject::STATE_CHAR_ATK_V:
//			if (distX < 100) {
//				self->pressDefend();
//			}
//			else if (distX < 130) {
//				GLint alt = AEUtil::randomIntBetween(0, 3);
//				if (alt < 2) {
//					self->pressDefend();
//				}
//				else {
//					self->input(AEKeyboardHandler::INPUT_2A);
//				}
//			}
//			else {
//				GLint alt = AEUtil::randomIntBetween(0, 3);
//				if (alt < 2) {
//					self->pressDefend();
//				}
//				else {
//					self->pressForward();
//				}
//			}
//			break;
//		case AEObject::STATE_CHAR_ATK:
//			if (distX < 80) {
//				GLint alt = AEUtil::randomIntBetween(0, 3);
//				if (alt < 3) {
//					self->pressDefend();
//				}
//				else {
//					self->input(AEKeyboardHandler::INPUT_2A);
//				}
//			}
//			else if (distX < 100) {
//				GLint alt = AEUtil::randomIntBetween(0, 3);
//				if (alt < 3) {
//					self->input(AEKeyboardHandler::INPUT_2A);
//				}
//				else {
//					self->input(AEKeyboardHandler::INPUT_A);
//				}
//			}
//			else if (distX < 140) {
//				GLint alt = AEUtil::randomIntBetween(0, 3);
//				if (alt < 1) {
//					self->pressForward();
//				}
//				else {
//					self->input(AEKeyboardHandler::INPUT_8A);
//				}
//			}
//			else {
//				self->pressForward();
//			}
//			break;
//		default:
//			if (distX < 70) {
//				GLint alt = AEUtil::randomIntBetween(0, 4);
//				if (alt < 2) {
//					self->pressBackward();
//				}
//				else if (alt < 3) {
//					self->input(AEKeyboardHandler::INPUT_2A);
//				}
//				else {
//					self->input(AEKeyboardHandler::INPUT_A);
//				}
//			}
//			else if (distX < 105) {
//				if (self->getState() == AEObject::STATE_CHAR_ATK) {
//					self->input(AEKeyboardHandler::INPUT_8A);
//				}
//				else {
//					GLint alt = AEUtil::randomIntBetween(0, 3);
//					if (alt < 2) {
//						self->input(AEKeyboardHandler::INPUT_A);
//					}
//					else {
//						self->input(AEKeyboardHandler::INPUT_2A);
//					}
//				}
//			}
//			else if (distX < 130) {
//				GLint alt = AEUtil::randomIntBetween(0, 5);
//				if (alt < 3) {
//					self->input(AEKeyboardHandler::INPUT_8A);
//				}
//				else if (alt < 4) {
//					self->pressForward();
//				}
//			}
//			else {
//				self->pressForward();
//			}
//		}
//		break;
//
//	case TYPE_SWORDMAN_TRAINER:
//
//		for (GLint i = 0; i < sTable.getHashCount(); i++) {
//			AESprite* s = sTable.getByHash(i);
//			if (oTable.get(s->getOid())->getType() == OBJ_PROJECTILE) {
//				if (floor(checkdistXWith(s) / s->getVx()) < 15 && s->getState() == AEObject::STATE_PROJ_FLY) {
//					self->pressDefend();
//					return;
//				}
//			}
//		}
//		if (target->getState() == AEObject::STATE_CHAR_ATK || target->getState() == AEObject::STATE_CHAR_ATK_H) {
//			self->pressDefend();
//		}
//		else if (target->getState() == AEObject::STATE_CHAR_ATK_V) {
//			self->pressUp();
//			self->pressDefend();
//		}
//		break;
//
//	default:
//
//		break;
//
//	}
//
//}

AEAITable::AEAITable() {
	maxIndex = pHash = 0;
	for (int i = 0; i < MAX_ONLINE_AIS; i++) {
		occupied[i] = 0;
		hash[i] = -1;
	}
}

GLvoid AEAITable::add(AEAI* ai) {
	for (int i = 0; i <= maxIndex; i++) {
		if (!occupied[i]) {
			ai->setIndex(i);
			table[i] = ai;  occupied[i] = 1;
			hash[pHash] = i;  pHash++;
			return;
		}
	}
	if (maxIndex == AESpriteTable::MAX_ONLINE_SPRITES - 1) {
		printf("Too many sprites online ..\n");
	}
	else {
		maxIndex++;
		ai->setIndex(maxIndex);
		table[maxIndex] = ai;  occupied[maxIndex] = 1;
		hash[pHash] = maxIndex;  pHash++;
	}
}

GLvoid AEAITable::addAt(GLint index, AEAI* ai) {
	if (occupied[index]) {
		printf("Error on adding sprite: Slot Occupied.\n");
		return;
	}
	if (index > maxIndex) {
		maxIndex = index;
	}
	ai->setIndex(index);
	table[index] = ai;  occupied[index] = 1;
	hash[pHash] = maxIndex;  pHash++;
}

GLvoid AEAITable::remove(GLint index) {
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
		}
	}
}

GLvoid AEAITable::clear() {
	for (int i = 0; i <= maxIndex; i++) {
		if (occupied[i])
			remove(i);
	}
}

GLvoid AEAITable::update() {
	for (int i = 0; i <= maxIndex; i++) {
		if (occupied[i] && table[i]->isEnabled())
			table[i]->execute();
	}
}