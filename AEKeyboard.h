#pragma once

class AEKeyboardHandler {

public:

	static const GLint INPUT_JUDGE_TOLERATION	= 5;
	static const GLint OPKEY_COUNT_TOTAL		= 7;
	static const GLint KS_NONE					= 0x0;
	static const GLint AKS_LEFT					= 0x1;
	static const GLint AKS_RIGHT				= 0x2;
	static const GLint AKS_UP					= 0x4;
	static const GLint AKS_DOWN					= 0x8;
	static const GLint BKS_ATTACK				= 0x10;
	static const GLint BKS_JUMP					= 0x20;
	static const GLint BKS_DEFEND				= 0x40;
	static const GLint INPUT_COUNT				= 13;
	static const GLint INPUT_8					= 0;
	static const GLint INPUT_2					= 1;
	static const GLint INPUT_4					= 2;
	static const GLint INPUT_6					= 3;
	static const GLint INPUT_A					= 4;
	static const GLint INPUT_J					= 5;
	static const GLint INPUT_D					= 6;
	static const GLint INPUT_8A					= 7;
	static const GLint INPUT_2A					= 8;
	static const GLint INPUT_4A					= 9;
	static const GLint INPUT_6A					= 10;
	static const GLint INPUT_236A				= 11;
	static const GLint INPUT_8D					= 12;

	static void keyDown(unsigned char key);
	static void keyUp(unsigned char key);

private:



};