#ifndef CHUNKS_H
#define CHUNKS_H
#include "Voxel.h"
#include "VoxelRender.h"
#include "Chunk.h"

class Chunk;
class voxel;
class Chunks
{
private:
	Chunk** chunks;
	Chunk** secondchunks;
	Mesh** meshes;
	Mesh** secondmeshes;

	int ox, oy, oz;
	size_t volume;
public:
	uint8_t w, h, d;
	Chunks(const uint& w, const uint& h, const uint&  d,
		const uint& ox = 0, const uint& oy = 0, const uint& oz = 0);
	const size_t& Getvolume();
	unsigned char getLight(int x, int y, int z, int channel);
	Chunk* GetChunk(int i);
	Chunk* GetChunk(int x, int y, int z);
	voxel* Get(int x, int y, int z);
	voxel* rayCast(vec3 a, vec3 dir, float maxDistance, vec3& end, vec3& norm, vec3& iend);
	void Set(int x, int y, int z, int id);
	void write(unsigned char* info);
	void read(unsigned char* source);
	Chunk* GetChunkByVoxel(int x, int y, int z);

	void setCenter(int x, int y, int z);
	void Translate(int dx, int dy, int dz);

	bool LoadVisible();
	bool _buildMeshes(VoxelRender* renderer);

	~Chunks();
};

#endif