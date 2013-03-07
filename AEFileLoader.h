#pragma once

using namespace std;


class AEFileLoader {

public:

	static GLvoid loadBackground(AEBackground* bg, string bgDataFileName);
	static GLvoid loadParticleTex(AEParticleSystem* ps, GLint index, string texFileName);
	static GLvoid loadMainData(string dataFileName);
	static GLvoid loadObject(AEObject* obj, string objectName);
	static GLvoid loadObjectTable(string dataFileName);

private:



};