/**
 * @file	AEFileLoader.h
 * @brief	Providing with methods for different kinds of file-loading.
 *
 * @author	CaiyZ (Mr.IroN)
 * @version	0.1
 * @date	2013
*/

#pragma once

using namespace std;


class AEFileLoader {

public:

	static GLvoid loadBackground(AEBackground* bg, string bgDataFileName);
	static GLvoid loadParticleTex(AEParticleSystem* ps, GLint index, string texFileName);
	static GLvoid loadMainData(string dataFileName);
	static GLvoid loadObject(AEObject* obj, string objectName);
	static GLvoid loadObjectTable(string dataFileName);

};