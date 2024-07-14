#ifndef VOXEL_RENDER_H
#define VOXEL_RENDER_H
#include "lib.h"
#include "Defines.h"
#include "Chunk.h"
#include "Mesh.h"
#include "Voxel.h"
class Mesh;
class Chunk;
class VoxelRender
{
	int artts[4] = { 3, 2, 1, 0 };
	float* buffer;
	size_t capacity;
public:
	VoxelRender(size_t capacity);
	~VoxelRender();

	Mesh* render(Chunk* chunk);
};

#endif // !VOXEL_RENDER_H