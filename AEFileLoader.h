#pragma once

using namespace std;


class AEFileLoader {

public:

	static GLvoid loadMainData(string dataFileName);
	static GLvoid loadObject(AEObject* obj, string objectName);
	static GLvoid loadObjectTable(string dataFileName);

private:



};