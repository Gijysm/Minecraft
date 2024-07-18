#ifndef CHUNK_H
#define CHUNK_H
#include "lib.h"
#include "Defines.h"
#include "Voxel.h"
class voxel;

class Chunk
{
public:
	int x, y, z;
	voxel* voxels;
	bool modifier ;
	Chunk(int xpos, int ypos, int zpos);
	~Chunk();
	void initializeVoxels();
};

#endif // !CHUNK_H