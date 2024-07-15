#ifndef CHUNKS_H
#define CHUNKS_H
#include "Voxel.h"
#include "Chunk.h"
class Chunk;
class voxel;
class Chunks
{
private:
	Chunk** chunks;
	size_t volume;
public:
	uint w, h, d;
	Chunks(uint w, uint h, uint  d);
	const size_t& Getvolume();
	Chunk* GetChunk(int i);
	~Chunks();
};

#endif
