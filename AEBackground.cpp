#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <string>
#include <GL\glut.h>
#include "AESystemParam.h"
#include "AEImageLoader.h"
#include "AEUtility.h"
#include "AEBackground.h"

extern char* context;

AEBGLayer::AEBGLayer(GLint _rid, GLint _depth, GLint _width, GLint _height, GLfloat _offsetx, GLfloat _offsety) {
	rid = _rid;  depth = _depth;  width = _width;  height = _height;
	offsetPosition.x = _offsetx;  offsetPosition.y = _offsety;
	animCount = 0;
}

GLvoid AEBGLayerAnim::setFrameImage(GLint index, GLint _rid, GLint _offset, GLint _width, GLint _height) {
	frames[index].rid = _rid;
	frames[index].imgOffset = _offset;
	frames[index].width = _width;
	frames[index].height = _height;
}

AEBackground::AEBackground() {
	location.x = location.y = 0;
	layerCount = landformCount = 0;
}

GLint AEBackground::getLandformIndexBelow(GLint cx, GLint cy, GLint* drop) {
	GLint bg_x = cx - GLint(location.x);  GLint bg_y = cy - GLint(location.y);
	GLint index = -1, maxAltitude = -1;
	for (GLint i = 0; i < landformCount; i++) {
		if (bg_x >= landforms[i].left && bg_x < landforms[i].right && bg_y + AEBackground::ONLANDFORM_TOLERANCE >= landforms[i].data[bg_x].altitude) {
			if (landforms[i].data[bg_x].altitude > maxAltitude) {
				maxAltitude = landforms[i].data[bg_x].altitude;
				index = i;
			}
		}
	}
	if (index == -1)
		drop = NULL;
	else
		(*drop) = bg_y - maxAltitude;
	return index;
}

GLubyte offset2mask(GLint offset) {
	switch (offset) {
	case 0:
		return 0x80;
	case 1:
		return 0x40;
	case 2:
		return 0x20;
	case 3:
		return 0x10;
	case 4:
		return 0x08;
	case 5:
		return 0x04;
	case 6:
		return 0x02;
	case 7:
		return 0x01;
	default:
		return 0x00;
	}
}

GLbyte isWhiteMono(GLubyte currentByte, GLint bitOffset) {
	GLubyte mask = offset2mask(bitOffset);
	if ((currentByte & mask) > 0)
		return 1;
	else
		return 0;
}

GLvoid setBlackMono(GLubyte* pixels, GLint byteOffset, GLint bitOffset) {
	GLubyte mask = offset2mask(bitOffset);
	pixels[byteOffset] = pixels[byteOffset] & ~mask;
}

GLvoid AEBackground::loadLandformsFromMonochrome(GLubyte* pixels, GLint width, GLint height, GLint byteLine) {
	landformCount = 0;
	for (GLint x = 0; x < width; x++) {
		for (GLint y = 0; y < height; y++) {
			GLint byteLineOffset = x / 8;  GLint bitOffset = x % 8;
			if (isWhiteMono(pixels[y * byteLine + byteLineOffset], bitOffset)) {
				landforms[landformCount].left = x;
				landforms[landformCount].data = new LandformElem[width];
				for (GLint i = 0; i < width; i++) {
					landforms[landformCount].data[i].altitude = -1;
					landforms[landformCount].data[i].slope = 0.0;
				}
				landforms[landformCount].data[x].altitude = y;
				setBlackMono(pixels, y * byteLine + byteLineOffset, bitOffset);
				GLint x2 = x, y2 = y;
				while (x2 < width) {
					x2++;
					if (x2 >= width) {
						landforms[landformCount].right = width - 1;
						break;
					}
					GLint newbyteLineOffset = x2 / 8;  GLint newBitOffset = x2 % 8;
					if (y2 > 0 && isWhiteMono(pixels[(y2 - 1) * byteLine + newbyteLineOffset], newBitOffset)) {
						y2--;
						landforms[landformCount].data[x2].altitude = y2;
						setBlackMono(pixels, y2 * byteLine + newbyteLineOffset, newBitOffset);
					}
					else if (isWhiteMono(pixels[y2 * byteLine + newbyteLineOffset], newBitOffset)) {
						landforms[landformCount].data[x2].altitude = y2;
						setBlackMono(pixels, y2 * byteLine + newbyteLineOffset, newBitOffset);
					}
					else if (y2 < height - 1 && isWhiteMono(pixels[(y2 + 1) * byteLine + newbyteLineOffset], newBitOffset)) {
						y2++;
						landforms[landformCount].data[x2].altitude = y2;
						setBlackMono(pixels, y2 * byteLine + newbyteLineOffset, newBitOffset);
					}
					else {
						landforms[landformCount].right = x2 - 1;
						break;
					}
				}
				landformCount++;
			}
		}
	}
	for (GLint i = 0; i < landformCount; i++) {
		for (GLint j = landforms[i].left; j <= landforms[i].right; j++) {
			GLint lp = j - 2;  if (lp < landforms[i].left) lp = landforms[i].left;
			GLint rp = j + 2;  if (rp > landforms[i].right) rp = landforms[i].right;
			landforms[i].data[j].slope = (landforms[i].data[rp].altitude - landforms[i].data[lp].altitude) / 4.0;
		}
	}
}

GLbyte isWhite(GLubyte* pixels, GLint x, GLint y, GLint width) {
	GLint pix = y * width + x;
	if (pixels[pix * 4 + 0] == 0x00 && pixels[pix * 4 + 1] == 0x00 && pixels[pix * 4 + 2] == 0x00)
		return 0;
	else
		return 1;
}

GLvoid setBlack(GLubyte* pixels, GLint x, GLint y, GLint width) {
	GLint pix = y * width + x;
	pixels[pix * 4 + 0] = pixels[pix * 4 + 1] = pixels[pix * 4 + 2] = pixels[pix * 4 + 3] = 0;
}

GLvoid AEBackground::loadLandforms(GLubyte* pixels, GLint width, GLint height) {
	landformCount = 0;
	for (GLint x = 0; x < width; x++) {
		for (GLint y = 0; y < height; y++) {
			if (isWhite(pixels, x, y, width)) {
				landforms[landformCount].left = x;
				landforms[landformCount].data = new LandformElem[width];
				for (GLint i = 0; i < width; i++) {
					landforms[landformCount].data[i].altitude = -1;
					landforms[landformCount].data[i].slope = 0.0;
				}
				landforms[landformCount].data[x].altitude = y;
				setBlack(pixels, x, y, width);
				GLint x2 = x, y2 = y;
				while (x2 < width) {
					x2++;
					if (x2 >= width) {
						landforms[landformCount].right = width - 1;
						break;
					}
					if (y2 > 0 && isWhite(pixels, x2, y2 - 1, width)) {
						y2--;
						landforms[landformCount].data[x2].altitude = y2;
						setBlack(pixels, x2, y2, width);
					}
					else if (isWhite(pixels, x2, y2, width)) {
						landforms[landformCount].data[x2].altitude = y2;
						setBlack(pixels, x2, y2, width);
					}
					else if (y2 < height - 1 && isWhite(pixels, x2, y2 + 1, width)) {
						y2++;
						landforms[landformCount].data[x2].altitude = y2;
						setBlack(pixels, x2, y2, width);
					}
					else {
						landforms[landformCount].right = x2 - 1;
						break;
					}
				}
				landformCount++;
			}
		}
	}
	for (GLint i = 0; i < landformCount; i++) {
		for (GLint j = landforms[i].left; j <= landforms[i].right; j++) {
			GLint lp = j - 2;  if (lp < landforms[i].left) lp = landforms[i].left;
			GLint rp = j + 2;  if (rp > landforms[i].right) rp = landforms[i].right;
			landforms[i].data[j].slope = (landforms[i].data[rp].altitude - landforms[i].data[lp].altitude) / 4.0;
		}
	}
}

GLvoid AEBackground::addAnimAt(GLint index, AEBGLayerAnim layerAnim) {
	if (index < AEBackground::MAX_ANIM_COUNT && index >= 0) {
		animTable[index] = layerAnim;
	}
}

GLvoid AEBackground::update() {
	for (GLint i = 0; i < layerCount; i++) {
		for (GLint j = 0; j < layer[i]->getAnimCount(); j++) {
			layer[i]->incTimeForAnim(j);
			AEBGLayerAnim anim = animTable[layer[i]->getAnimRef(j).animIndex];
			if (layer[i]->getTimeOfAnim(j) >= anim.getEndTime(layer[i]->getFrameOfAnim(j))) {
				layer[i]->incFrameForAnim(j);
				GLint t = anim.getFrameCount();
				if (layer[i]->getFrameOfAnim(j) == anim.getFrameCount()) {
					layer[i]->resetAnim(j);
				}
			}
		}
	}
}

GLvoid AEBackground::paint(AEPoint cameraCenter) {
	AERect texClip, cameraRect;
	cameraRect.x1 = cameraCenter.x - AESysParam::SCREEN_WIDTH / 2.0;
	cameraRect.x2 = cameraRect.x1 + AESysParam::SCREEN_WIDTH;
	cameraRect.y1 = cameraCenter.y - AESysParam::SCREEN_HEIGHT / 2.0;
	cameraRect.y2 = cameraRect.y1 + AESysParam::SCREEN_HEIGHT;
	for (GLint i = layerCount - 1; i >= 0; i--) {
		GLfloat dx = cameraCenter.x;
		GLfloat depth = layer[i]->getDepth();
		GLfloat correction = dx * depth / 100.0;
 		texClip.x1 = (cameraRect.x1 - location.x - correction - layer[i]->getOffsetPosition().x) / layer[i]->getWidth();
		texClip.x2 = texClip.x1 + 1.0 * AESysParam::SCREEN_WIDTH / layer[i]->getWidth();
		texClip.y1 = (cameraRect.y1 - location.y - layer[i]->getOffsetPosition().y) / layer[i]->getHeight();
		texClip.y2 = texClip.y1 + 1.0 * AESysParam::SCREEN_HEIGHT / layer[i]->getHeight();
		AEUtil::paintRect(bgResTable.get(layer[i]->getLayerRid())->getTexture(), texClip, cameraRect);
		for (GLint j = 0; j < layer[i]->getAnimCount(); j++) {
			AnimRef ref = layer[i]->getAnimRef(j);
			AEBGLayerAnim lanim = animTable[ref.animIndex];
			BGLayerFrame lf = lanim.getFrame(ref.frame);
			AEResource* res = bgResTable.get(lf.rid);
			AERect animRect;
			animRect.x1 = location.x + layer[i]->getOffsetPosition().x + ref.x + correction;
			animRect.x2 = animRect.x1 + lf.width;
			animRect.y1 = location.y + layer[i]->getOffsetPosition().y + ref.y;
			animRect.y2 = animRect.y1 + lf.height;
			AEUtil::paintRect(bgResTable.get(lf.rid)->getTexture(), res->getTexCoords(lf.imgOffset, 1), animRect);
		}
	}
}

AEBackgroundLibrary::AEBackgroundLibrary() {
	maxIndex = 0;
	for (int i = 0; i < MAX_BG_COUNT; i++) {
		lib[i] = NULL;
	}
}

GLvoid AEBackgroundLibrary::add(AEBackground* bg) {
	if (maxIndex > MAX_BG_COUNT) {
		// Error
		return;
	}
	lib[maxIndex] = bg;
	maxIndex++;
}