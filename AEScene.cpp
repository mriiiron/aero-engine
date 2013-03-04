#include <GL\glut.h>
#include "AESystemParam.h"
#include "AEUtility.h"
#include "AECamera.h"
#include "AEScene.h"

extern AECamera camera;

AEScene::AEScene() {
	bg = NULL;
	sTable = NULL;
	hud = NULL;
}

GLvoid AEScene::run() {
	

}

GLvoid AEScene::stop() {


}

GLvoid AEScene::update() {
	// ptclSys.update();
	sTable->update();
	hud->update();
	bg->update();
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

GLvoid AESceneManager::addAt(GLint index, AEScene* scene) {
	if (index < 0 || index > MAX_SCENE_NUM || scene == NULL) {
		// Error
	}
	table[index] = scene;
}