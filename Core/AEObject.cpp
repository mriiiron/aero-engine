#define _CRT_SECURE_NO_WARNINGS

/*******************************************************************
== File Info ==
Name:				objects.cpp
Implementions of:	objects.h
Author:				CaiyZ (Mr.IroN)

== File Comment ==
This file contains the implementions of "objects.h".
When fetching graphical data, it may refer to Resource Table.
Moreover the Fonts are also loaded while loading Objects.
*******************************************************************/

#include <stdio.h>
#include <fstream>
#include <GL\glut.h>
#include "AESystemParam.h"
#include "AEUtility.h"
#include "AEKeyboard.h"
#include "AEResource.h"
#include "AEObject.h"

using namespace std;

extern AEResourceTable rTable;
extern AEObjectTable oTable;


GLbyte AEObject::keyStrToByte(string keyStr) {
	if (keyStr == "none") {
		return AEKeyboardHandler::KS_NONE;
	}
	else if (keyStr == "attack") {
		return AEKeyboardHandler::BKS_ATTACK;
	}
	else if (keyStr == "jump") {
		return AEKeyboardHandler::BKS_JUMP;
	}
	else if (keyStr == "defend") {
		return AEKeyboardHandler::BKS_DEFEND;
	}
	else if (keyStr == "up") {
		return AEKeyboardHandler::AKS_UP;
	}
	else if (keyStr == "down") {
		return AEKeyboardHandler::AKS_DOWN;
	}
	else if (keyStr == "left") {
		return AEKeyboardHandler::AKS_LEFT;
	}
	else if (keyStr == "right") {
		return AEKeyboardHandler::AKS_RIGHT;
	}
	return 0;
}

GLbyte AEObject::keyStrToInputCode(string keyStr) {
	if (keyStr == "i_8") {
		return AEKeyboardHandler::INPUT_8;
	}
	else if (keyStr == "i_2") {
		return AEKeyboardHandler::INPUT_2;
	}
	else if (keyStr == "i_4") {
		return AEKeyboardHandler::INPUT_4;
	}
	else if (keyStr == "i_6") {
		return AEKeyboardHandler::INPUT_6;
	}
	else if (keyStr == "i_A") {
		return AEKeyboardHandler::INPUT_A;
	}
	else if (keyStr == "i_J") {
		return AEKeyboardHandler::INPUT_J;
	}
	else if (keyStr == "i_D") {
		return AEKeyboardHandler::INPUT_D;
	}
	else if (keyStr == "i_8A") {
		return AEKeyboardHandler::INPUT_8A;
	}
	else if (keyStr == "i_2A") {
		return AEKeyboardHandler::INPUT_2A;
	}
	else if (keyStr == "i_4A") {
		return AEKeyboardHandler::INPUT_4A;
	}
	else if (keyStr == "i_6A") {
		return AEKeyboardHandler::INPUT_6A;
	}
	else if (keyStr == "i_236A") {
		return AEKeyboardHandler::INPUT_236A;
	}
	else if (keyStr == "i_8D") {
		return AEKeyboardHandler::INPUT_8D;
	}
	return 0;
}