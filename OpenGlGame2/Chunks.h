#ifndef CHUNKS_H
#define CHUNKS_H
#include "Voxel.h"
#include "Chunk.h"

class Chunk;
class voxel;
class Chunks
{
private:

	size_t volume;
public:
	Chunk** chunks;

	uint w, h, d;
	Chunks(uint w, uint h, uint  d);
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
	~Chunks();
};

#endif