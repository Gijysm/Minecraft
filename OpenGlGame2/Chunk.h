#ifndef CHUNK_H
#define CHUNK_H
#include "lib.h"
#include "Defines.h"
#include "Voxel.h"
#include "LightMap.h"
class voxel;
class LightMap;
class Chunk
{
public:
	int16 x, y, z;
	LightMap* lightmap;
	voxel* voxels;
	bool modifier = true;
	Chunk(int& xpos, int& ypos, int& zpos);
	~Chunk();
	bool isEmpty();
	void initializeVoxels();
};

#endif // !CHUNK_H