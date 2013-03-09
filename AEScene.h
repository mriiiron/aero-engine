#pragma once

#include "AEHud.h"
#include "AEBackground.h"
#include "AESprite.h"


class AEScene {

public:

	AEScene();
	GLvoid addBackground(AEBackground* _bg, GLfloat x, GLfloat y) { bg = _bg;  bg->setLocation(x, y); }
	GLvoid addSpriteTable(AESpriteTable* _sTable) { sTable = _sTable; }
	GLvoid addHUD(AEHeadUpDisplay* _hud) { hud = _hud; }
	AEBackground* getBackground() { return bg; }
	AESpriteTable* getSpriteTable() { return sTable; }
	AEHeadUpDisplay* getHUD() { return hud; }
	GLvoid update();
	GLvoid paint();

	virtual GLvoid keyDown(GLubyte key, int x, int y) = 0;

private:

	AEBackground* bg;
	AESpriteTable* sTable;
	AEHeadUpDisplay* hud;

};


class AESceneManager {

public:

	static const GLint MAX_SCENE_NUM			= 20;
	static const GLint SCENE_NONE				= -1;

	AESceneManager();
	GLvoid addSceneAt(GLint index, AEScene* scene);
	GLvoid stopAll();
	GLvoid run(GLint index);
	AEScene* getActiveScene() { return table[activeSceneIndex]; }

private:

	AEScene* table[MAX_SCENE_NUM];
	GLint activeSceneIndex;

};