#ifndef CHUNK_H
#define CHUNK_H
#include "lib.h"
#include "Defines.h"
#include "Voxel.h"
class voxel;

class Chunk
{
public:
	voxel* voxels;
	Chunk();
	~Chunk();
	void initializeVoxels();
};

#endif // !CHUNK_H