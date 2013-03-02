#pragma once


typedef struct _Text {

	Font font;
	GLint x, y;

} Text;


class AEHeadUpDisplay {

public:

	static const GLint MAX_TEXT_COUNTS			= 30;

	AEHeadUpDisplay();
	GLvoid addText(GLint x, GLint y, Font font);
	GLvoid update();
	GLvoid paint();

private:

	Text texts[MAX_TEXT_COUNTS];
	GLint maxTextIndex;

};