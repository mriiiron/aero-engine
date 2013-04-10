#pragma once

#include <GL\glut.h>


template <typename T, GLint MAX_ELEM_COUNT>
class AEHashedTable {

public:

	AEHashedTable();
	T* getItem(GLint index) { return table[index]; }
	T* getItemByHash(GLint hashIndex) { return table[hash[hashIndex]]; }
	GLint getHash(GLint hashIndex) { return hash[hashIndex]; }
	GLint getHashCount() { return pHash; }
	GLvoid add(T* t);
	GLvoid addAt(GLint index, T* t);
	GLvoid remove(GLint index);
	GLvoid clear();

private:

	T* table[MAX_ELEM_COUNT];
	GLint occupied[MAX_ELEM_COUNT];
	GLint hash[MAX_ELEM_COUNT];
	GLint maxIndex, pHash;

};


template <typename T, GLint MAX_ELEM_COUNT>
class AETable {

public:

	AETable();
	GLvoid addAt(GLint index, T* t);
	T* get(GLint index) { return table[index]; }

private:

	T* table[MAX_ELEM_COUNT];
	GLint occupied[MAX_ELEM_COUNT];

};


template <typename T, GLint MAX_ELEM_COUNT>
AEHashedTable<T, MAX_ELEM_COUNT>::AEHashedTable() {
	maxIndex = -1;
	pHash = 0;
	for (int i = 0; i < MAX_ELEM_COUNT; i++) {
		occupied[i] = 0;
		hash[i] = -1;
	}
}

template <typename T, GLint MAX_ELEM_COUNT>
GLvoid AEHashedTable<T, MAX_ELEM_COUNT>::add(T* t) {
	for (GLint i = 0; i <= maxIndex; i++) {
		if (!occupied[i]) {
			//t->setIndex(i);
			table[i] = t;  occupied[i] = 1;
			hash[pHash] = i;  pHash++;
			return;
		}
	}
	if (maxIndex == MAX_ELEM_COUNT - 1) {
		printf("Error on adding to hashed table: Too many items.\n");
	}
	else {
		maxIndex++;
		//t->setIndex(maxIndex);
		table[maxIndex] = t;  occupied[maxIndex] = 1;
		hash[pHash] = maxIndex;  pHash++;
	}
}

template <typename T, GLint MAX_ELEM_COUNT>
GLvoid AEHashedTable<T, MAX_ELEM_COUNT>::addAt(GLint index, T* t) {
	if (occupied[index]) {
		printf("Error on adding to hashed table: Slot occupied.\n");
		return;
	}
	if (index > maxIndex) {
		maxIndex = index;
	}
	//t->setIndex(index);
	table[index] = t;  occupied[index] = 1;
	hash[pHash] = maxIndex;  pHash++;
}

template <typename T, GLint MAX_ELEM_COUNT>
GLvoid AEHashedTable<T, MAX_ELEM_COUNT>::remove(GLint index) {
	if (!occupied[index]) {
		printf("Error on removing from hashed table: Slot empty.\n");
		return;
	}
	delete table[index];
	table[index] = NULL;
	occupied[index] = 0;
	while (!occupied[maxIndex] && maxIndex > 0) maxIndex--;
	for (int i = 0; i < pHash; i++) {
		if (hash[i] == index) {
			pHash--;
			hash[i] = hash[pHash];
			hash[pHash] = -1;
			break;
		}
	}
}

template <typename T, GLint MAX_ELEM_COUNT>
GLvoid AEHashedTable<T, MAX_ELEM_COUNT>::clear() {
	for (int i = 0; i <= maxIndex; i++) {
		if (occupied[i])
			remove(i);
	}
}

template <typename T, GLint MAX_ELEM_COUNT>
AETable<T, MAX_ELEM_COUNT>::AETable() {
	for (GLint i = 0; i < MAX_ELEM_COUNT; i++) {
		occupied[i] = 0;
	}
}

template <typename T, GLint MAX_ELEM_COUNT>
GLvoid AETable<T, MAX_ELEM_COUNT>::addAt(GLint index, T* t) {
	if (occupied[index]) {
		return;
	}
	table[index] = t;
	occupied[index] = 1;
}