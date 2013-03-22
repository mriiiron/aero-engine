#include <stdio.h>
#include <windows.h>
#include <string>
#include <time.h>
#include <GL\glut.h>
#include "AEroEngine.h"

#include "AETScene.h"

using namespace std;

#define FPS_PLANNED				40

extern AEResourceTable rTable;
extern AEObjectTable oTable;
extern AEBackgroundLibrary bgLib;
extern AEFontLibrary fontLib;
extern AEParticleSystem ptclSys;
extern AESceneManager sceneManager;
extern AECamera camera;

GLint showCrosshair = 0;

GLvoid exitGame(const char* info) {
	printf("%s", info);
	system("pause");
	exit(EXIT_SUCCESS);
}

GLvoid glInit() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	gluOrtho2D(-320.0, 320.0, -240.0, 240.0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

GLvoid gameInit() {
	srand(unsigned(time(NULL)));
	AESysParam::init();
	AEFileLoader::loadMainData("res\\maindata.txt");
	AEFileLoader::loadObjectTable("res\\objects.txt");
}

GLvoid createTestWorld() {
	AEScene* testTitleScene = new AETTitleScene();
	AEHeadUpDisplay* titleHud = new AEHeadUpDisplay();
	titleHud->addText(220, 100, 0, string("-- PRESS START --"));
	titleHud->addImage(320, 240, rTable.get(0), 0);
	testTitleScene->addHUD(titleHud);
	sceneManager.addSceneAt(0, testTitleScene);
	AEScene* testScene = new AETMainScene();
	AEHeadUpDisplay* hud = new AEHeadUpDisplay();
	hud->addText(100, 150, 0, string("fuck world"));
	testScene->addHUD(hud);
	testScene->addBackground(bgLib.get(0), -600.0, -220.0);
	sceneManager.addSceneAt(1, testScene);
	sceneManager.runScene(0);
}

// glTranslatef(camera.getCrosshair().x, camera.getCrosshair().y, 0.0);

GLfloat fps;
GLint fps_frames = 0;
GLint fps_time = 0;
GLint fps_timebase = 0;

GLvoid paintScene() {
	sceneManager.getActiveScene()->paint();
	// ptclSys.paint();
	fps_frames++;
	fps_time = glutGet(GLUT_ELAPSED_TIME);
	if (fps_time - fps_timebase > 1000) {
		fps = fps_frames * 1000.0 / (fps_time - fps_timebase);
		fps_timebase = fps_time;		
		fps_frames = 0;
	}
	fontLib.getFont(0)->writeAt(5, 5, "Current FPS: %.1f\n", fps);
}

DWORD time1, time2;

GLvoid updateScene() {
	sceneManager.getActiveScene()->update();
}

GLvoid onDisplay() {
	DWORD dtime;
	long sleepTime;
	updateScene();
	glClear(GL_COLOR_BUFFER_BIT);
	paintScene();
	glutSwapBuffers();
	time2 = GetTickCount();
	dtime = time2 - time1;
	sleepTime = 1000 / FPS_PLANNED - dtime;
	if (sleepTime < 2) {
		sleepTime = 2;
	}
	Sleep(sleepTime);
	time1 = GetTickCount();
	glutPostRedisplay();
}

GLvoid onReshape(GLint w, GLint h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

GLvoid onKeyDown(GLubyte key, int x, int y) {
	sceneManager.getActiveScene()->keyDown(key, x, y);
}

//GLvoid onKeyUp(GLubyte key, int x, int y) {
//	AEKeyboardHandler::keyUp(key);
//}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("AEro Engine by CaiyZ (Mr.IroN)");
	glutDisplayFunc(&onDisplay);
	glutReshapeFunc(&onReshape);
	glutKeyboardFunc(&onKeyDown);
	// glutKeyboardUpFunc(&onKeyUp);
	// glutSpecialFunc(&onSpecialKeyDown);
	// glutSpecialUpFunc(&onSpecialKeyUp);
	glInit();
	gameInit();
	createTestWorld();
	time1 = GetTickCount();
	glutMainLoop();
	return 0;
}