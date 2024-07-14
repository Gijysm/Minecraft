#include "VoxelRender.h"
#define _VERTEX_SIZE (3+2+1) // Враховуючи додатковий атрибут для освітлення
#define IS_IN(X,Y,Z) ((X) >= 0 && (X) < _CHUNK_W && (Y) >= 0 && (Y) < _CHUNK_H && (Z) >= 0 && (Z) < _CHUNK_D)
#define VOXEL(X, Y, Z) (chunk->voxels[(Y) * _CHUNK_D * _CHUNK_W + (Z) * _CHUNK_W + (X)])
#define IS_BLOCKED(X, Y, Z) (IS_IN(X, Y, Z) && VOXEL(X, Y, Z).id)
#define VERTEX(INDEX, X, Y, Z, U, V, L) \
    buffer[INDEX + 0] = X;           \
    buffer[INDEX + 1] = Y;           \
    buffer[INDEX + 2] = Z;           \
    buffer[INDEX + 3] = U;           \
    buffer[INDEX + 4] = V;           \
    buffer[INDEX + 5] = L;           \
    INDEX += _VERTEX_SIZE;
VoxelRender::VoxelRender(size_t capacity) : capacity(capacity)
{
buffer = new float[capacity * _VERTEX_SIZE * 6];
}

VoxelRender::~VoxelRender()
{
	delete [] buffer;
}
Mesh* VoxelRender::render(Chunk* chunk)
{
	size_t index = 0;
	for (int y = 0; y < _CHUNK_H; y++) {
		for (int z = 0; z < _CHUNK_D; z++) {
			for (int x = 0; x < _CHUNK_W; x++) {
				voxel vox = chunk->voxels[(y * _CHUNK_D + z) * _CHUNK_W + x];
				unsigned int id = vox.id;

				if (!id) {
					continue;
				}

				float l;
				float uvsize = 1.0f / 32.0f;
				float u = (id % 16) * uvsize;
				float v = 1 - ((1 + id / 16) / uvsize);

				if (!IS_BLOCKED(x, y + 1, z)) {
					l = 1.0f;
					VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u + uvsize, v, l);
					VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);

					VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u + uvsize, v, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u, v, l);
				}
				if (!IS_BLOCKED(x, y - 1, z)) {
					l = 0.75f;
					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
					VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
					VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u, v + uvsize, l);

					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
					VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
					VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
				}

				if (!IS_BLOCKED(x + 1, y, z)) {
					l = 0.95f;
					VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u + uvsize, v + uvsize, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);

					VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);
					VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u, v, l);
				}
				if (!IS_BLOCKED(x - 1, y, z)) {
					l = 0.85f;
					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
					VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
					VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u, v + uvsize, l);

					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
					VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u + uvsize, v, l);
					VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
				}

				if (!IS_BLOCKED(x, y, z + 1)) {
					l = 0.9f;
					VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u, v, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
					VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);

					VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u, v, l);
					VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u + uvsize, v, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
				}
				if (!IS_BLOCKED(x, y, z - 1)) {
					l = 0.8f;
					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
					VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u + uvsize, v + uvsize, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u, v + uvsize, l);

					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u, v + uvsize, l);
					VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u, v, l);
				}
			}
		}
	}
	return new Mesh(buffer, index / _VERTEX_SIZE, artts);
}
