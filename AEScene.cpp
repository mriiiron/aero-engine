#include <GL\glut.h>
#include "AEUtility.h"
#include "AECamera.h"
#include "AEBackground.h"
#include "AEHud.h"
#include "AESprite.h"
#include "AEAI.h"
#include "AEScene.h"


AEScene::AEScene() {
	camera = NULL;
	bg = NULL;
	sTable = NULL;
	aTable = NULL;
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
	camera->update();
}

GLvoid AEScene::paint() {
	if (bg != NULL) {
		bg->paint(camera->getCrosshair());
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