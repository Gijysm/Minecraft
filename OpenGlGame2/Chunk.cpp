#include "Chunk.h"

Chunk::Chunk()
{
	voxels = new voxel[_CHUNK_SIZE];
	for(int y = 0; y < _CHUNK_H; y++)
	{
		for(int z = 0; z < _CHUNK_D; z++)
		{
			for(int x = 0; x < _CHUNK_W; x++)
			{
				voxels[x + z * _CHUNK_W + y * _CHUNK_W * _CHUNK_D].id = y <= (sin(x*0.6) *0.5 +0.5) * 5;
			}
		}
	}
}

Chunk::~Chunk()
{
	delete [] voxels;
}
