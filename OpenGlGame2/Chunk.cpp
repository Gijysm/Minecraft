#include "Chunk.h"

Chunk::Chunk(int& xpos, int& ypos, int& zpos) : x(xpos), y(ypos), z(zpos)
{
	lightmap = new LightMap();
	voxels = new voxel[_CHUNK_SIZE];
	for (int z = 0; z < _CHUNK_D; z++) {
		for (int x = 0; x < _CHUNK_W; x++) {
			int16 real_x = x + this->x * _CHUNK_W;
			int16 real_z = z + this->z * _CHUNK_D;
			//float height = glm::perlin(glm::vec3(real_x*0.0026125f,real_z*0.0026125f, 0.0f));
			//height += glm::perlin(glm::vec3(real_x*0.006125f,real_z*0.006125f, 0.0f))*0.5f;
			for (int y = 0; y < _CHUNK_H; y++) {
				int16 real_y = y + this->y * _CHUNK_H;
				int16 id = glm::perlin(glm::vec3(real_x * 0.0125f, real_y * 0.0125f, real_z * 0.0125f)) > 0.1f;//real_y <= (height) * 60 + 30;
				if (real_y <= 2)
					id = 2;
				voxels[(y * _CHUNK_D + z) * _CHUNK_W + x].id = id;
			}
		}
	}
}

Chunk::~Chunk()
{
	delete lightmap;
	delete [] voxels;
}

bool Chunk::isEmpty()
{
	if()
}

