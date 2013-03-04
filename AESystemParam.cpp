#include <GL\glut.h>
#include "AESystemParam.h"

GLvoid AESysParam::init() {
	aiTimer = 0;
	gameState = GAMESTATE_TITLE;
}