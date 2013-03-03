#pragma once


class AEScene {

public:

	AEScene();
	GLvoid addBackground(AEBackground* _bg) { bg = _bg; }
	GLvoid addSpriteTable(AESpriteTable* _sTable) { sTable = _sTable; }
	GLvoid addHUD(AEHeadUpDisplay* _hud) { hud = _hud; }
	AEBackground* getBackground() { return bg; }
	AESpriteTable* getSpriteTable() { return sTable; }
	AEHeadUpDisplay* getHUD() { return hud; }
	GLvoid run();
	GLvoid stop();
	GLvoid update();
	GLvoid paint();

private:

	AEBackground* bg;
	AESpriteTable* sTable;
	AEHeadUpDisplay* hud;

};


class AESceneManager {

public:

	static const GLint MAX_SCENE_NUM			= 20;

	GLvoid addAt(GLint index, AEScene* scene);

private:

	AEScene* table[MAX_SCENE_NUM];

};