#include "Chunk.h"

Chunk::Chunk()
{
	voxels = new voxel[_CHUNK_SIZE];
	for (int y = 0; y < _CHUNK_H; y++) {
		for (int z = 0; z < _CHUNK_D; z++) {
			for (int x = 0; x < _CHUNK_W; x++) {
				int id = y <= (sin(x * 0.6f) * 0.5f + 0.5f) * 10;
				if (y <= 2)
					id = 2;
				voxels[(y * _CHUNK_D + z) * _CHUNK_W + x].id = id;
			}
		}
	}
}

Chunk::~Chunk()
{
	delete [] voxels;
}
