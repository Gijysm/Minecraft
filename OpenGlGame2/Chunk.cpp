#include "Chunk.h"

Chunk::Chunk(int xpos, int ypos, int zpos) : x(xpos), y(ypos), z(zpos)
{
	voxels = new voxel[_CHUNK_SIZE];
	for (int y = 0; y < _CHUNK_H; y++) {
		for (int z = 0; z < _CHUNK_D; z++) {
			for (int x = 0; x < _CHUNK_W; x++) {
				int real_x = x + this->x * _CHUNK_W;
				int real_y = y + this->y * _CHUNK_H;
				int real_z = z + this->z * _CHUNK_D;
				int id = real_y <= (sin(real_x * 0.6f) * 0.5f + 0.5f) * 10;
				if (real_y <= 2)
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
