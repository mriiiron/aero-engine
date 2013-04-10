#include <iostream>
#include <GL\glut.h>
#include "AEroEngine.h"
#include "SPCaster.h"
#include "SPEnemy.h"
#include "SPParticleEffect.h"
#include "SPLightning.h"
#include "SPScene.h"

extern AEParticleSystem ptclSys;
extern AESceneManager sceneManager;

SPTitleScene::SPTitleScene() {
	fadeInTimer = 60;  fadeInStep = 1.0f / fadeInTimer;
	holdTimer = 60;
	fadeOutTimer = 60;  fadeOutStep = 1.0f / fadeOutTimer;
	logoAlpha = 0.0;
}

GLvoid SPTitleScene::update() {
	if (fadeInTimer > 0) {
		logoAlpha += fadeInStep;
		fadeInTimer--;
	}
	else if (holdTimer > 0) {
		logoAlpha = 1.0f;
		holdTimer--;
	}
	else if (fadeOutTimer > 0) {
		logoAlpha -= fadeOutStep;
		fadeOutTimer--;
	}
	else {
		sceneManager.runScene(1);
	}
	AEScene::update();
}

GLvoid SPTitleScene::paint() {
	glColor4f(1.0, 1.0, 1.0, logoAlpha);
	AEScene::paint();
	glColor4f(1.0, 1.0, 1.0, 1.0);
}

GLvoid SPTitleScene::keyDown(GLubyte key, int x, int y) {

}

GLvoid SPMainScene::update() {
	AEScene::update();
	for (int i = 0; i < spellTable->getHashCount(); i++) {
		SPSpell* spell = spellTable->getItemByHash(i);
		if (spell->isDead()) {
			spellTable->remove(spellTable->getHash(i));
		}
		else {
			spell->update();
		}
	}
}

GLvoid SPMainScene::paint() {
	AEScene::paint();
	glDisable(GL_TEXTURE_2D);
	for (int i = 0; i < spellTable->getHashCount(); i++) {
		spellTable->getItemByHash(i)->paint();
	}
	glEnable(GL_TEXTURE_2D);
}

GLvoid SPMainScene::init() {
	spellTable = new AEHashedTable<SPSpell, 200>;
	SPCaster* davis = new SPCaster(this, 0, AESprite::TEAM_1, -250, -182);
	SPEnemy* golden_1 = new SPEnemy(this, 1, AESprite::TEAM_2, 250, -80, 1, AESprite::CAST_INVERSE);
	//SPEnemy* golden_2 = new SPEnemy(this, 1, AESprite::TEAM_2, 295, -130, 1, AESprite::CAST_INVERSE);
	//SPEnemy* gray_1 = new SPEnemy(this, 1, AESprite::TEAM_2, 270, -170, 15, AESprite::CAST_INVERSE);
	//golden_1->setVx(0.3f);
	davis->setTarget(golden_1);
	//golden_2->setVx(0.3f);
	sTable->add(davis);
	sTable->add(golden_1);
	//sTable->add(golden_2);
	//sTable->add(gray_1);
}

GLvoid SPMainScene::keyDown(GLubyte key, int x, int y) {
	if (key == 'a') {
		sTable->get(0)->changeAction(2);
	}
	else if (key == 's') {
		spellTable->add(new SPLightning(AEUtil::createPoint(200.0f, 300.0f), AEUtil::createPoint(200.0f, -185.0f), 1, 30, 8.0f));
		ptclSys.generate(new SPElectricShock(200.0f, -185.0f, 0.0f, 100));
	}
}