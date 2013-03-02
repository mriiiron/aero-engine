#pragma once


class AEScene {

public:

	AEScene();
	GLvoid addCamera(AECamera* _camera) { camera = _camera; }
	GLvoid addBackground(AEBackground* _bg) { bg = _bg; }
	GLvoid addSpriteTable(AESpriteTable* _sTable) { sTable = _sTable; }
	GLvoid addAITable(AEAITable* _aTable) { aTable = _aTable; }
	GLvoid addHUD(AEHeadUpDisplay* _hud) { hud = _hud; }
	AECamera* getCamera() { return camera; }
	AEBackground* getBackground() { return bg; }
	AESpriteTable* getSpriteTable() { return sTable; }
	AEAITable* getAITable() { return aTable; }
	AEHeadUpDisplay* getHUD() { return hud; }
	GLvoid run();
	GLvoid stop();
	GLvoid update();
	GLvoid paint();

private:

	AECamera* camera;
	AEBackground* bg;
	AESpriteTable* sTable;
	AEAITable* aTable;
	AEHeadUpDisplay* hud;

};


class AESceneManager {

public:

	static const GLint MAX_SCENE_NUM			= 20;

	GLvoid addAt(GLint index, AEScene* scene);

private:

	AEScene* table[MAX_SCENE_NUM];

};