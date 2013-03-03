#include <GL\glut.h>
#include "AESystemParam.h"
#include "AEKeyboard.h"

GLvoid AEKeyboardHandler::keyDown(unsigned char key) {

}

GLvoid AEKeyboardHandler::keyUp(unsigned char key) {

}

//GLvoid onKeyDown(unsigned char key, int x, int y) {
//	switch (AESysParam::gameState) {
//	case GAME_STATE_TITLE:
//		switch (key) {
//		case ' ':
//			mainLoopInit();
//			gameState = GAME_STATE_MAINLOOP;
//			glutPostRedisplay();
//			break;
//		case 'q':
//			printf("<q> Just a break ..\n");  break;
//		default:
//			break;
//		}
//		break;
//	case GAME_STATE_MAINLOOP:
//		switch (key) {
//		case 'z':
//			hero->pressAttack();  break;
//		case 'c':
//			hero->pressJump();  break;
//		case 'x':
//			hero->pressDefend();  break;
//		case 'q':
//			printf("<q> Just a break ..\n");  break;
//		case 'w':
//			ptclSys.generate(new ParticleEmitter(EMIT_TYPE_EXPLODE, EMIT_MODE_ERUPT, 0.0, 0.0, 0.0, 40));  break;
//		case 'e':
//			foe->input(INPUT_8A);  break;
//		case 'r':
//			showCrosshair = 1 - showCrosshair;  break;
//		case ',':
//			camera.pressLeft();  break;
//		case '.':
//			camera.pressRight();  break;
//		default:
//			break;
//		}
//		break;
//	default:
//		exitGame("Undefined Game State.\n");
//		break;
//	}
//}
//
//GLvoid onKeyUp(unsigned char key, int x, int y) {
//	switch (gameState) {
//	case GAME_STATE_TITLE:
//		break;
//	case GAME_STATE_MAINLOOP:
//		switch (key) {
//		case 'z':
//			hero->releaseAttack();  break;
//		case 'c':
//			hero->releaseJump();  break;
//		case 'x':
//			hero->releaseDefend();  break;
//		case ',':
//			camera.releaseLeft();  break;
//		case '.':
//			camera.releaseRight();  break;
//		default:
//			break;
//		}
//	break;
//	default:
//		break;
//	}
//}
//
//GLvoid onSpecialKeyDown(int key, int x, int y) {
//	switch (gameState) {
//	case GAME_STATE_TITLE:
//		switch (key) {
//		case GLUT_KEY_UP:
//			titleSelector--;
//			if (titleSelector < 0) titleSelector += TITLE_OPTIONS_COUNT;
//			break;
//		case GLUT_KEY_DOWN:
//			titleSelector++;
//			if (titleSelector >= TITLE_OPTIONS_COUNT) titleSelector -= TITLE_OPTIONS_COUNT;
//			break;
//		case GLUT_KEY_LEFT:
//			switch (titleSelector) {
//			case TITLE_SELECTOR_MODE:
//				gameMode--;
//				if (gameMode < 0) gameMode += GAME_MODE_COUNT;
//				break;
//			case TITLE_SELECTOR_1P:
//				_1pSelector--;
//				switch (gameMode) {
//				case GAME_MODE_HUMAN_VS_AI:
//					if (_1pSelector < 0) _1pSelector += CHAR_COUNT;
//					break;
//				case GAME_MODE_AI_VS_AI:
//					if (_1pSelector < 0) _1pSelector += AI_COUNT;
//					break;
//				default:
//					exitGame("Illegal Game Mode.\n");
//					break;
//				}
//				break;
//			case TITLE_SELECTOR_2P:
//				_2pSelector--;
//				if (_2pSelector < 0) _2pSelector += AI_COUNT;
//				break;
//			default:
//				break;
//			}
//			break;
//		case GLUT_KEY_RIGHT:
//			switch (titleSelector) {
//			case TITLE_SELECTOR_MODE:
//				gameMode++;
//				if (gameMode >= GAME_MODE_COUNT) gameMode -= GAME_MODE_COUNT;
//				break;
//			case TITLE_SELECTOR_1P:
//				_1pSelector++;
//				switch (gameMode) {
//				case GAME_MODE_HUMAN_VS_AI:
//					if (_1pSelector >= CHAR_COUNT) _1pSelector -= CHAR_COUNT;
//					break;
//				case GAME_MODE_AI_VS_AI:
//					if (_1pSelector >= AI_COUNT) _1pSelector -= AI_COUNT;
//					break;
//				default:
//					exitGame("Illegal Game Mode.\n");
//					break;
//				}
//				break;
//			case TITLE_SELECTOR_2P:
//				_2pSelector++;
//				if (_2pSelector >= AI_COUNT) _2pSelector -= AI_COUNT;
//				break;
//			default:
//				break;
//			}
//			break;
//		default:
//			break;
//		}
//		glutPostRedisplay();
//		break;
//	case GAME_STATE_MAINLOOP:
//		switch (key) {
//		case GLUT_KEY_UP:
//			hero->pressUp();
//			break;
//		case GLUT_KEY_DOWN:
//			hero->pressDown();
//			break;
//		case GLUT_KEY_LEFT:
//			hero->pressLeft();
//			break;
//		case GLUT_KEY_RIGHT:
//			hero->pressRight();
//			break;
//		case GLUT_KEY_F1:
//			gameState = GAME_STATE_TITLE;
//			gameInit();
//			break;
//		case GLUT_KEY_F2:
//			camera.changeTarget();
//			break;
//		default:
//			break;
//		}
//		break;
//	default:
//		exitGame("Undefined Game State.\n");
//		break;
//	}
//}
//
//GLvoid onSpecialKeyUp(int key, int x, int y) {
//	switch (gameState) {
//	case GAME_STATE_TITLE:
//		break;
//	case GAME_STATE_MAINLOOP:
//		switch (key) {
//		case GLUT_KEY_UP:
//			hero->releaseUp();
//			break;
//		case GLUT_KEY_DOWN:
//			hero->releaseDown();
//			break;
//		case GLUT_KEY_LEFT:
//			hero->releaseLeft();
//			break;
//		case GLUT_KEY_RIGHT:
//			hero->releaseRight();
//			break;
//		default:
//			break;
//		}
//		break;
//	default:
//		break;
//	}
//}