/**
 * @file	AEImageLoader.h
 * @brief	Providing with methods for image loading.
 *
 * (requires FreeImage.lib)
 *
 * @author	CaiyZ (Mr.IroN)
 * @version	0.1
 * @date	2013
*/

#pragma once


class AEImageLoader {

public:

	static GLuint loadBMPTexture(const char* fileName);
	static GLubyte* loadMonochromeBMP(const char* fileName, GLint* pWidth, GLint* pHeight, GLint* pByteLine);
	static GLubyte* loadPNG(const char* fileName, GLint* pWidth, GLint* pHeight);
	static GLubyte* loadPNG(const char* fileName);
	static GLuint loadPNGTexture(const char* fileName, GLint* pWidth, GLint* pHeight);
	static GLuint loadPNGTexture(const char* fileName);

};