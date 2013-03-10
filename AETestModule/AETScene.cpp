#include <iostream>
#include <GL\glut.h>
#include "AEroEngine.h"
#include "AETScene.h"

extern AESceneManager sceneManager;

GLvoid AETTitleScene::keyDown(GLubyte key, int x, int y) {
	if (key == 'a') {
		sceneManager.runScene(1);
	}
	else {
		std::cout << "fuck you" << std::endl;
	}
}

GLvoid AETMainScene::keyDown(GLubyte key, int x, int y) {
	std::cout << "fuck you all!!" << std::endl;
}