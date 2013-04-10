#include <iostream>
#include <windows.h>
#include <string>
#include <time.h>
#include <GL\glut.h>
#include "AEroEngine.h"
#include "SPScene.h"

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
	cout << info << endl;
	system("pause");
	exit(EXIT_SUCCESS);
}

GLvoid glInit() {
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("Spellbinder Demo");
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	gluOrtho2D(-320.0, 320.0, -240.0, 240.0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

GLvoid gameInit() {
	srand(unsigned(time(NULL)));
	AESysParam::init();
	AEFileLoader::loadMainData("res\\maindata.txt");
	AEFileLoader::loadObjectTable("res\\objects.txt");
}

GLvoid createWorld() {
	SPTitleScene* titleScene = new SPTitleScene();
	AEHeadUpDisplay* titleHUD = new AEHeadUpDisplay();
	titleHUD->addImage(320, 240, rTable.get(0), 0);
	titleScene->addHUD(titleHUD);
	sceneManager.addSceneAt(0, titleScene);
	SPMainScene* mainScene = new SPMainScene();
	mainScene->addBackground(bgLib.get(0), -600, -220);
	mainScene->addSpriteTable(new AESpriteTable());
	mainScene->init();
	sceneManager.addSceneAt(1, mainScene);
	sceneManager.runScene(1);
}

// glTranslatef(camera.getCrosshair().x, camera.getCrosshair().y, 0.0);

GLfloat fps;
GLint fps_frames = 0;
GLint fps_time = 0;
GLint fps_timebase = 0;

GLvoid paintScene() {
	AEScene* activeScene = sceneManager.getActiveScene();
	if (activeScene == NULL) {
		exitGame("No active scene.\n");
	}
	activeScene->paint();
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
	AEScene* activeScene = sceneManager.getActiveScene();
	if (activeScene == NULL) {
		exitGame("No active scene.\n");
	}
	activeScene->update();
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

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glInit();
	glutDisplayFunc(&onDisplay);
	glutReshapeFunc(&onReshape);
	glutKeyboardFunc(&onKeyDown);
	gameInit();
	createWorld();
	time1 = GetTickCount();
	glutMainLoop();
	return 0;
}