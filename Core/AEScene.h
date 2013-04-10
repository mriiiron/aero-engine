/**
 * @file	AEScene.h
 * @brief	Defining the game scenes.
 *
 * The scenes are almost the most important components in a game created using AEro Engine.
 * Each scene could own its own background, sprite table and HUD. Meanwhile different scenes
 * can react differently to the same input. For example, when you click somewhere in an "menu"
 * scene, a menu item might be selected; but if you click on your enemy in the "gameplay" scene,
 * your hero would just start to shoot.
 *
 * @author	CaiyZ (Mr.IroN)
 * @version	0.1
 * @date	2013
*/

#pragma once

#include "AEHud.h"
#include "AEBackground.h"
#include "AESprite.h"


class AEScene {

public:

	AEScene();
	GLvoid addBackground(AEBackground* _bg, GLfloat x, GLfloat y) { bg = _bg;  bg->setLocation(x, y); }
	GLvoid addSpriteTable(AESpriteTable* _sTable) { sTable = _sTable; }
	GLvoid addSprite(AESprite* _sp);
	GLvoid addHUD(AEHeadUpDisplay* _hud) { hud = _hud; }
	AEBackground* getBackground() { return bg; }
	AESpriteTable* getSpriteTable() { return sTable; }
	AEHeadUpDisplay* getHUD() { return hud; }

	virtual GLvoid update();
	virtual GLvoid paint();
	virtual GLvoid keyDown(GLubyte key, int x, int y) = 0;

protected:

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
	GLvoid runScene(GLint index);
	AEScene* getActiveScene() { if (activeSceneIndex >= 0) return table[activeSceneIndex]; else return NULL; }

private:

	AEScene* table[MAX_SCENE_NUM];
	GLint activeSceneIndex;

};