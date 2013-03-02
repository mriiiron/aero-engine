#pragma once		


typedef struct _LandformElem {
	GLint altitude;
	GLfloat slope;
} LandformElem;

typedef struct _Landform {
	LandformElem* data;
	GLint left, right;
} Landform;

typedef struct _BGLayerFrame {
	GLint width, height;
	GLint rid;
	GLint imgOffset;
} BGLayerFrame;

class AEBGLayerAnim {

public:

	AEBGLayerAnim() { frames = NULL;  endTimes = NULL;  frameCount = 0; }
	GLvoid setFrameImage(GLint index, GLint _rid, GLint _offset, GLint _width, GLint _height);
	GLvoid setFrameCount(GLint _frameCount) { frameCount = _frameCount;  frames = new BGLayerFrame[_frameCount];  endTimes = new GLint[_frameCount];}
	GLvoid setEndTime(GLint index, GLint _endTime) { endTimes[index] = _endTime; }
	BGLayerFrame getFrame(GLint index) { return frames[index]; }
	GLint getEndTime(GLint index) { return endTimes[index]; }
	GLint getFrameCount() { return frameCount; }

private:

	BGLayerFrame* frames;
	GLint* endTimes;
	GLint frameCount;

};

typedef struct _AnimRef {
	GLint x, y;
	GLint animIndex;
	GLint time, frame;
} AnimRef;

class AEBGLayer {

public:

	static const GLint MAX_LAYER_ANIMS			= 20;

	AEBGLayer(GLint _rid, GLint _depth, GLint _width, GLint _height, GLfloat _offsetx, GLfloat _offsety);
	GLint getLayerRid() { return rid; }
	GLint getDepth() { return depth; }
	GLint getWidth() { return width; }
	GLint getHeight() { return height; }
	GLint getAnimCount() { return animCount; }
	GLint getTimeOfAnim(GLint index) { return anim[index].time; }
	GLint getFrameOfAnim(GLint index) { return anim[index].frame; }
	AnimRef getAnimRef(GLint index) { return anim[index]; }
	AEPoint getOffsetPosition() { return offsetPosition; }
	GLvoid addAnimRef(AnimRef animRef) { anim[animCount] = animRef;  animCount++; }
	GLvoid incTimeForAnim(GLint index) { anim[index].time = anim[index].time + 1; }
	GLvoid incFrameForAnim(GLint index) { anim[index].frame = anim[index].frame + 1; }
	GLvoid resetAnim(GLint index) { anim[index].time = anim[index].frame = 0; }

private:

	GLint rid, depth, width, height;
	AEPoint offsetPosition;
	GLint animCount;
	AnimRef anim[MAX_LAYER_ANIMS];

};

class AEBackground {

public:

	static const GLint MAX_LAYERS				= 10;
	static const GLint MAX_ANIMS				= 50;
	static const GLint MAX_LANDFORMS			= 50;
	static const GLint ONLANDFORM_TOLERANCE		= 2;

	AEBackground();
	GLint getXonBG(GLint cx) { return cx - GLint(location.x); }
	GLint getYonBG(GLint cy) { return cy - GLint(location.y); }
	GLint getLandformIndexBelow(GLint cx, GLint cy, GLint* drop);
	Landform getLandform(GLint index) { return landforms[index]; }
	GLvoid loadLandformsFromMonochrome(GLubyte* pixels, GLint width, GLint height, GLint byteLine);
	GLvoid loadLandforms(GLubyte* pixels, GLint width, GLint height);
	GLvoid loadFromFile(const char* fileName);
	GLvoid addAnimAt(GLint index, AEBGLayerAnim layerAnim);
	GLvoid update();
	GLvoid paint(AEPoint cameraCenter);
	AEPoint getLocation() { return location; }
	GLint getWidth() { return width; }
	GLint getHeight() { return height; }
	GLvoid setName(const char* _name) { strcpy(name, _name); }
	GLvoid setLocation(GLfloat _x, GLfloat _y) { location.x = _x;  location.y = _y; }
	GLvoid setWidth(GLint _width) { width = _width; }
	GLvoid setHeight(GLint _height) { height = _height; }
	GLvoid setLayerCount(GLint _layerCount) { layerCount = _layerCount; }

private:

	char name[50];
	AEPoint location;
	GLint width, height;
	GLint layerCount, landformCount;
	AEBGLayer* layer[MAX_LAYERS];
	Landform landforms[MAX_LANDFORMS];
	AEResourceTable bgResTable;
	AEBGLayerAnim animTable[MAX_ANIMS];

};



class AEBackgroundLibrary {

public:

	static const GLint MAX_BG_COUNT				= 30;

	AEBackgroundLibrary();
	GLvoid add(AEBackground* bg);

private:

	AEBackground* lib[MAX_BG_COUNT];
	GLint maxIndex;

};