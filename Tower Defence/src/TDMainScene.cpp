#include <GL\glut.h>
#include "AEroEngine.h"
#include "TDTurret.h"
#include "TDTank.h"
#include "TDAI.h"
#include "TDMainScene.h"

extern AESceneManager sceneManager;
extern AEBackgroundLibrary bgLib;
extern AEAITable aiTable;

GLvoid TDMainScene::init() {
	addBackground(bgLib.get(0), -600, -270);
	AESpriteTable* sTable = new AESpriteTable();
	TDTurret* turret_1 = new TDTurret(this, 2, AESprite::TEAM_1, 0, 170);
	TDTurret* turret_2 = new TDTurret(this, 2, AESprite::TEAM_1, -40, -220);
	TDTurret* turret_3 = new TDTurret(this, 2, AESprite::TEAM_1, -100, 150);
	TDTurretAI* ai_1 = new TDTurretAI(turret_1);
	TDTurretAI* ai_2 = new TDTurretAI(turret_2);
	TDTurretAI* ai_3 = new TDTurretAI(turret_3);
	ai_1->enable();
	ai_2->enable();
	ai_3->enable();
	sTable->add(turret_1);
	sTable->add(turret_2);
	sTable->add(turret_3);
	aiTable.add(ai_1);
	aiTable.add(ai_2);
	aiTable.add(ai_3);
	addSpriteTable(sTable);
}

GLvoid TDMainScene::keyDown(GLubyte key, int x, int y) {
	if (key == 'a') {
		TDTank* newTank = new TDTank(this, 3, AESprite::TEAM_2, -225, -65, 0);
		newTank->changeAction(1);
		sTable->add(newTank);
	}
	else if (key == 's') {
		TDTank* newTank = new TDTank(this, 3, AESprite::TEAM_2, 225, 0, 0, AESprite::CAST_INVERSE);
		newTank->changeAction(1);
		sTable->add(newTank);
	}
	else {
		GLint a;
		a = 0;
	}
}