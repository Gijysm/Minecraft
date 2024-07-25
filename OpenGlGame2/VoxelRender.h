#ifndef VOXEL_RENDER_H
#define VOXEL_RENDER_H
#include "lib.h"
#include "Defines.h"
#include "Chunk.h"
#include "Mesh.h"
#include "Voxel.h"
#include "Block.h"
#include "LightMap.h"
class Mesh;
class Chunk;
class VoxelRender
{
	float32 lr, lg, lb, ls;
	float32 lr0, lr1, lr2, lr3;
	float32 lg0, lg1, lg2, lg3;
	float32 lb0, lb1, lb2, lb3;
	float32 ls0, ls1, ls2, ls3;
	int artts[4] = { 3, 2, 4, 0 };
	float* buffer;
	size_t capacity;
public:
	VoxelRender(size_t capacity);
	~VoxelRender();

	Mesh* render(Chunk* chunk, const Chunk** chunks);
};

#endif // !VOXEL_RENDER_H