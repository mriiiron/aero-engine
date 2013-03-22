#include <GL\glut.h>
#include "AESystemParam.h"
#include "AEUtility.h"
#include "AECamera.h"
#include "AEKeyboard.h"
#include "AEScene.h"

extern AECamera camera;

AEScene::AEScene() {
	bg = NULL;
	sTable = NULL;
	hud = NULL;
}

GLvoid AEScene::update() {
	// ptclSys.update();
	if (bg != NULL) {
		bg->update();
	}
	if (sTable != NULL) {
		sTable->update();
	}
	if (hud != NULL) {
		hud->update();
	}
	//camera->update();
	//AESysParam::aiTimer--;
	//if (AESysParam::aiTimer == 0) {
	//	aTable->update();
	//	AESysParam::aiTimer = AEAI::EXEC_INTERVAL;
	//}
}

GLvoid AEScene::paint() {
	if (bg != NULL) {
		bg->paint(camera.getCrosshair());
	}
	if (sTable != NULL) {
		sTable->paint();
	}
	if (hud != NULL) {
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0.0, 640.0, 0, 480.0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		hud->paint();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}
}

AESceneManager::AESceneManager() {
	for (GLint i = 0; i < MAX_SCENE_NUM; i++) {
		table[i] = NULL;
	}
	activeSceneIndex = SCENE_NONE;
}

GLvoid AESceneManager::addSceneAt(GLint index, AEScene* scene) {
	if (index < 0 || index > MAX_SCENE_NUM || scene == NULL) {
		// Error
		return;
	}
	if (table[index] != NULL) {
		// Error
		return;
	}
	table[index] = scene;
}

GLvoid AESceneManager::stopAll() {
	activeSceneIndex = SCENE_NONE;
}

GLvoid AESceneManager::runScene(GLint index) {
	activeSceneIndex = index;
}