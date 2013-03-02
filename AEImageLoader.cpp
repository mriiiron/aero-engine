#define _CRT_SECURE_NO_WARNINGS

// TODO: if file not found then ..

#include <stdio.h>
#include <FI\FreeImage.h>
#include <GL\glut.h>
#include "AEImageLoader.h"

GLuint loadBMPTexture(const char* fileName) {
	GLint width, height, byteTotal;
	GLubyte* pixels = 0;
	GLuint texture = 0;
	FILE* pFile = fopen(fileName, "rb");
	printf("Loading texture from: %s .. ", fileName);
	if (pFile == 0) {
		printf("Error: Image file not found.\n");
		return -1;
	}
	else {
		printf("Done.\n");
	}
	fseek(pFile, 0x0012, SEEK_SET);
	fread(&width, 4, 1, pFile);
	fread(&height, 4, 1, pFile);
	{
		GLint byteLine = width * 3;
		while (byteLine % 4 != 0)
			byteLine++;
		byteTotal = byteLine * height;
	}
	pixels = new GLubyte[byteTotal];
	if (pixels == 0) {
		fclose(pFile);
		return -1;
	}
	fseek(pFile, 0x0036, SEEK_SET);
	if (fread(pixels, 1, byteTotal, pFile) <= 0) {
		delete[] pixels;
		fclose(pFile);
		return -1;
	}
	glGenTextures(1, &texture);
	if (texture == 0) {
		delete[] pixels;
		fclose(pFile);
		return -1;
	}
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	delete[] pixels;
	return texture;
}

GLubyte* loadMonochromeBMP(const char* fileName, GLint* pWidth, GLint* pHeight, GLint* pByteLine) {
	GLint dataStartPos, byteTotal;
	GLubyte* pixels = 0;
	FILE* pFile = fopen(fileName, "rb");
	if (pFile == 0) {
		printf("Error: Image file not found.\n");
		return 0;
	}
	fseek(pFile, 0x0012, SEEK_SET);
	fread(pWidth, 4, 1, pFile);
	fread(pHeight, 4, 1, pFile);
	fseek(pFile, 0x000A, SEEK_SET);
	fread(&dataStartPos, 4, 1, pFile);
	*pByteLine = (*pWidth) / 8;
	GLint remainder = (*pWidth) % 8;
	if (remainder > 0) (*pByteLine)++;
	while ((*pByteLine) % 4 != 0)
		(*pByteLine)++;
	byteTotal = (*pByteLine) * (*pHeight);
	pixels = new GLubyte[byteTotal];
	if (pixels == 0) {
		fclose(pFile);
		return 0;
	}
	fseek(pFile, dataStartPos, SEEK_SET);
	if (fread(pixels, 1, byteTotal, pFile) <= 0) {
		delete[] pixels;
		fclose(pFile);
		return 0;
	}
	return pixels;
}

GLubyte* loadPNG(const char* fileName, GLint* pWidth, GLint* pHeight) {
	FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(fileName, 0);
	FIBITMAP* dib = FreeImage_Load(fifmt, fileName, 0);
	BYTE *pixels = (BYTE*)FreeImage_GetBits(dib);
	(*pWidth) = FreeImage_GetWidth(dib);
	(*pHeight) = FreeImage_GetHeight(dib);
	return pixels;
}

GLubyte* loadPNG(const char* fileName) {
	FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(fileName, 0);
	FIBITMAP* dib = FreeImage_Load(fifmt, fileName, 0);
	BYTE *pixels = (BYTE*)FreeImage_GetBits(dib);
	return pixels;
}

// Code From: http://hi.baidu.com/new/pro_lily
GLuint loadPNGTexture(const char* fileName, GLint* pWidth, GLint* pHeight) {
	printf("from %s .. ", fileName);
	GLuint texture;
	FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(fileName, 0);
	FIBITMAP* dib = FreeImage_Load(fifmt, fileName, 0);
	(*pWidth) = FreeImage_GetWidth(dib);
	(*pHeight) = FreeImage_GetHeight(dib);
	BYTE *bits = new BYTE[FreeImage_GetWidth(dib) * FreeImage_GetHeight(dib) * 4];
	BYTE *pixels = (BYTE*)FreeImage_GetBits(dib);
	BYTE t_r, t_g, t_b, t_a;
	for (unsigned int pix = 0; pix < FreeImage_GetWidth(dib) * FreeImage_GetHeight(dib); pix++) {
	   t_r = bits[pix * 4 + 0] = pixels[pix * 4 + 2];
	   t_g = bits[pix * 4 + 1] = pixels[pix * 4 + 1];
	   t_b = bits[pix * 4 + 2] = pixels[pix * 4 + 0];
	   t_a = bits[pix * 4 + 3] = pixels[pix * 4 + 3];
	}
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), 0, GL_RGBA, GL_UNSIGNED_BYTE, bits);
	FreeImage_Unload(dib);
	delete[] bits;
	printf("Done.\n");
	return texture;
}

GLuint loadPNGTexture(const char* fileName) {
	printf("from %s .. ", fileName);
	GLuint texture;
	FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(fileName, 0);
	FIBITMAP* dib = FreeImage_Load(fifmt, fileName, 0);
	BYTE *bits = new BYTE[FreeImage_GetWidth(dib) * FreeImage_GetHeight(dib) * 4];
	BYTE *pixels = (BYTE*)FreeImage_GetBits(dib);
	BYTE t_r, t_g, t_b, t_a;
	for (unsigned int pix = 0; pix < FreeImage_GetWidth(dib) * FreeImage_GetHeight(dib); pix++) {
	   t_r = bits[pix * 4 + 0] = pixels[pix * 4 + 2];
	   t_g = bits[pix * 4 + 1] = pixels[pix * 4 + 1];
	   t_b = bits[pix * 4 + 2] = pixels[pix * 4 + 0];
	   t_a = bits[pix * 4 + 3] = pixels[pix * 4 + 3];
	}
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), 0, GL_RGBA, GL_UNSIGNED_BYTE, bits);
	FreeImage_Unload(dib);
	delete[] bits;
	printf("Done.\n");
	return texture;
}