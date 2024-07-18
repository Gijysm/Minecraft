#include "Chunk.h"

Chunk::Chunk(int xpos, int ypos, int zpos) : x(xpos), y(ypos), z(zpos)
{
	voxels = new voxel[_CHUNK_SIZE];
	for (int z = 0; z < _CHUNK_H; z++) {
		for (int x = 0; x < _CHUNK_D; x++) {
			int real_x = x + this->x * _CHUNK_W;
			int real_z = z + this->z * _CHUNK_D;
			float height = perlin(vec3(real_x * 0.05, real_z * 0.025, 0.0));
			for (int y = 0; y < _CHUNK_W; y++) {
				int real_y = y + this->y * _CHUNK_H;
				int id = real_y <= (height)  * 10 +5;
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
