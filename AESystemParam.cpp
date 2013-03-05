#include <GL\glut.h>
#include "AESystemParam.h"


GLint AESysParam::aiTimer;
GLint AESysParam::gameState;

GLvoid AESysParam::init() {
	aiTimer = 0;
	gameState = GAMESTATE_TITLE;
}