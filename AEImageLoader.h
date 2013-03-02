#pragma once

GLuint loadBMPTexture(const char* fileName);
GLubyte* loadMonochromeBMP(const char* fileName, GLint* pWidth, GLint* pHeight, GLint* pByteLine);
GLubyte* loadPNG(const char* fileName, GLint* pWidth, GLint* pHeight);
GLubyte* loadPNG(const char* fileName);
GLuint loadPNGTexture(const char* fileName, GLint* pWidth, GLint* pHeight);
GLuint loadPNGTexture(const char* fileName);