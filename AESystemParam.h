#pragma once

class AESysParam {

public:

	const static GLint MAX_CHAR_COUNT_IN_LINE	= 100;

	const static GLint SCREEN_WIDTH				= 640;
	const static GLint SCREEN_HEIGHT			= 480;

	const static GLint GAMESTATE_TITLE			= 0;

	static GLint aiTimer;
	static GLint gameState;

	static GLvoid init();

};