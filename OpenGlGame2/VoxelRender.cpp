#include "VoxelRender.h"

#define CDVI(X, A) (((X) < 0) ? ((X) / (A) - 1) : ((X) / (A)))
#define LOCAL_NEG(X, SIZE) (((X) < 0) ? ((X) + (SIZE)) : (X))
#define _VERTEX_SIZE (3+2+4)
#define IS_IN(X,Y,Z) ((X) >= 0 && (X) < _CHUNK_W && (Y) >= 0 && (Y) < _CHUNK_H && (Z) >= 0 && (Z) < _CHUNK_D)
#define VOXEL(X, Y, Z) (GET_CHUNK(X,Y,Z)->voxels[(LOCAL(Y, _CHUNK_H) * _CHUNK_D + LOCAL(Z, _CHUNK_D)) * _CHUNK_W + LOCAL(X, _CHUNK_W)])
#define LOCAL(X, SIZE) ((X) >= (SIZE) ? ((X) - (SIZE)) : LOCAL_NEG(X, SIZE))
#define GET_CHUNK(X, Y, Z) (chunks[((CDVI(Y, _CHUNK_H) + 1) * 3 + CDVI(Z, _CHUNK_D) + 1) * 3 + CDVI(X, _CHUNK_W) + 1])
#define IS_CHUNK(X,Y,Z) (GET_CHUNK(X,Y,Z) != nullptr)
#define IS_BLOCKED(X, Y, Z) ((!IS_CHUNK(X, Y, Z)) || VOXEL(X, Y, Z).id)
#define LIGHT(X,Y,Z,CHANNEL) (IS_CHUNK(X, Y, Z) ? GET_CHUNK(X, Y, Z)->lightmap->get(LOCAL(X, _CHUNK_W), LOCAL(Y, _CHUNK_H), LOCAL(Z, _CHUNK_D), (CHANNEL)) : 0)

#define VERTEX(INDEX, X, Y, Z, U, V, R, G, B, S) \
    buffer[INDEX + 0] = X; \
    buffer[INDEX + 1] = Y; \
    buffer[INDEX + 2] = Z; \
    buffer[INDEX + 3] = U; \
    buffer[INDEX + 4] = V; \
    buffer[INDEX + 5] = R; \
    buffer[INDEX + 6] = G; \
    buffer[INDEX + 7] = B; \
    buffer[INDEX + 8] = S; \
    INDEX += _VERTEX_SIZE;

VoxelRender::VoxelRender(size_t capacity) : capacity(capacity)
{
buffer = new float[capacity * _VERTEX_SIZE * 6];
}

VoxelRender::~VoxelRender()
{
	delete [] buffer;
}
Mesh* VoxelRender::render(Chunk* chunk, const Chunk** chunks)
{
	float aoFactor = 0.15;
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
				float uvsize = 1.f / 16.0f;
				float u1 = (float)(id % 512) * uvsize;
				float u2 = u1 + uvsize;
				float v1 = (id / 512) * uvsize;
				float v2 = v1 + uvsize;
				printf("ID: %d, u1: %f, u2: %f, v1: %f, v2: %f\n", id, u1, u2, v1, v2);

				if (!IS_BLOCKED(x, y + 1, z)) {
					l = 1.0f;
					lr = LIGHT(x, y + 1, z, 0) / 15.0f;
					lg = LIGHT(x, y + 1, z, 1) / 15.0f;
					lb = LIGHT(x, y + 1, z, 2) / 15.0f;
					ls = LIGHT(x, y + 1, z, 3) / 15.0f;

					lr0 = (LIGHT(x - 1, y + 1, z, 0) + lr * 30 + LIGHT(x - 1, y + 1, z - 1, 0) + LIGHT(x, y + 1, z - 1, 0)) / 5.0f / 15.0f;
					lr1 = (LIGHT(x - 1, y + 1, z, 0) + lr * 30 + LIGHT(x - 1, y + 1, z + 1, 0) + LIGHT(x, y + 1, z + 1, 0)) / 5.0f / 15.0f;
					lr2 = (LIGHT(x + 1, y + 1, z, 0) + lr * 30 + LIGHT(x + 1, y + 1, z + 1, 0) + LIGHT(x, y + 1, z + 1, 0)) / 5.0f / 15.0f;
					lr3 = (LIGHT(x + 1, y + 1, z, 0) + lr * 30 + LIGHT(x + 1, y + 1, z - 1, 0) + LIGHT(x, y + 1, z - 1, 0)) / 5.0f / 15.0f;

					lg0 = (LIGHT(x - 1, y + 1, z, 1) + lg * 30 + LIGHT(x - 1, y + 1, z - 1, 1) + LIGHT(x, y + 1, z - 1, 1)) / 5.0f / 15.0f;
					lg1 = (LIGHT(x - 1, y + 1, z, 1) + lg * 30 + LIGHT(x - 1, y + 1, z + 1, 1) + LIGHT(x, y + 1, z + 1, 1)) / 5.0f / 15.0f;
					lg2 = (LIGHT(x + 1, y + 1, z, 1) + lg * 30 + LIGHT(x + 1, y + 1, z + 1, 1) + LIGHT(x, y + 1, z + 1, 1)) / 5.0f / 15.0f;
					lg3 = (LIGHT(x + 1, y + 1, z, 1) + lg * 30 + LIGHT(x + 1, y + 1, z - 1, 1) + LIGHT(x, y + 1, z - 1, 1)) / 5.0f / 15.0f;

					lb0 = (LIGHT(x - 1, y + 1, z, 2) + lb * 30 + LIGHT(x - 1, y + 1, z - 1, 2) + LIGHT(x, y + 1, z - 1, 2)) / 5.0f / 15.0f;
					lb1 = (LIGHT(x - 1, y + 1, z, 2) + lb * 30 + LIGHT(x - 1, y + 1, z + 1, 2) + LIGHT(x, y + 1, z + 1, 2)) / 5.0f / 15.0f;
					lb2 = (LIGHT(x + 1, y + 1, z, 2) + lb * 30 + LIGHT(x + 1, y + 1, z + 1, 2) + LIGHT(x, y + 1, z + 1, 2)) / 5.0f / 15.0f;
					lb3 = (LIGHT(x + 1, y + 1, z, 2) + lb * 30 + LIGHT(x + 1, y + 1, z - 1, 2) + LIGHT(x, y + 1, z - 1, 2)) / 5.0f / 15.0f;

					ls0 = (LIGHT(x - 1, y + 1, z, 3) + ls * 30 + LIGHT(x - 1, y + 1, z - 1, 3) + LIGHT(x, y + 1, z - 1, 3)) / 5.0f / 15.0f;
					ls1 = (LIGHT(x - 1, y + 1, z, 3) + ls * 30 + LIGHT(x - 1, y + 1, z + 1, 3) + LIGHT(x, y + 1, z + 1, 3)) / 5.0f / 15.0f;
					ls2 = (LIGHT(x + 1, y + 1, z, 3) + ls * 30 + LIGHT(x + 1, y + 1, z + 1, 3) + LIGHT(x, y + 1, z + 1, 3)) / 5.0f / 15.0f;
					float ls3 = (LIGHT(x + 1, y + 1, z, 3) + ls * 30 + LIGHT(x + 1, y + 1, z - 1, 3) + LIGHT(x, y + 1, z - 1, 3)) / 5.0f / 15.0f;
					VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u2, v1, lr0, lg0, lb0, ls0);
					VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u2, v2, lr1, lg1, lb1, ls1);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u1, v2, lr2, lg2, lb2, ls2);
					VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u2, v1, lr0, lg0, lb0, ls0);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u1, v2, lr2, lg2, lb2, ls2);
					VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u1, v1, lr3, lg3, lb3, ls3);
				}
				if (!IS_BLOCKED(x, y - 1, z)) {
					l = 0.75f;
					lr = LIGHT(x, y - 1, z, 0) / 15.0f;
					lg = LIGHT(x, y - 1, z, 1) / 15.0f;
					lb = LIGHT(x, y - 1, z, 2) / 15.0f;
					ls = LIGHT(x, y - 1, z, 3) / 15.0f;

					lr0 = (LIGHT(x - 1, y - 1, z - 1, 0) + lr * 30 + LIGHT(x - 1, y - 1, z, 0) + LIGHT(x, y - 1, z - 1, 0)) / 5.0f / 15.0f;
					lr1 = (LIGHT(x + 1, y - 1, z + 1, 0) + lr * 30 + LIGHT(x + 1, y - 1, z, 0) + LIGHT(x, y - 1, z + 1, 0)) / 5.0f / 15.0f;
					lr2 = (LIGHT(x - 1, y - 1, z + 1, 0) + lr * 30 + LIGHT(x - 1, y - 1, z, 0) + LIGHT(x, y - 1, z + 1, 0)) / 5.0f / 15.0f;
					lr3 = (LIGHT(x + 1, y - 1, z - 1, 0) + lr * 30 + LIGHT(x + 1, y - 1, z, 0) + LIGHT(x, y - 1, z - 1, 0)) / 5.0f / 15.0f;

					lg0 = (LIGHT(x - 1, y - 1, z - 1, 1) + lg * 30 + LIGHT(x - 1, y - 1, z, 1) + LIGHT(x, y - 1, z - 1, 1)) / 5.0f / 15.0f;
					lg1 = (LIGHT(x + 1, y - 1, z + 1, 1) + lg * 30 + LIGHT(x + 1, y - 1, z, 1) + LIGHT(x, y - 1, z + 1, 1)) / 5.0f / 15.0f;
					lg2 = (LIGHT(x - 1, y - 1, z + 1, 1) + lg * 30 + LIGHT(x - 1, y - 1, z, 1) + LIGHT(x, y - 1, z + 1, 1)) / 5.0f / 15.0f;
					lg3 = (LIGHT(x + 1, y - 1, z - 1, 1) + lg * 30 + LIGHT(x + 1, y - 1, z, 1) + LIGHT(x, y - 1, z - 1, 1)) / 5.0f / 15.0f;

					lb0 = (LIGHT(x - 1, y - 1, z - 1, 2) + lb * 30 + LIGHT(x - 1, y - 1, z, 2) + LIGHT(x, y - 1, z - 1, 2)) / 5.0f / 15.0f;
					lb1 = (LIGHT(x + 1, y - 1, z + 1, 2) + lb * 30 + LIGHT(x + 1, y - 1, z, 2) + LIGHT(x, y - 1, z + 1, 2)) / 5.0f / 15.0f;
					lb2 = (LIGHT(x - 1, y - 1, z + 1, 2) + lb * 30 + LIGHT(x - 1, y - 1, z, 2) + LIGHT(x, y - 1, z + 1, 2)) / 5.0f / 15.0f;
					lb3 = (LIGHT(x + 1, y - 1, z - 1, 2) + lb * 30 + LIGHT(x + 1, y - 1, z, 2) + LIGHT(x, y - 1, z - 1, 2)) / 5.0f / 15.0f;

					ls0 = (LIGHT(x - 1, y - 1, z - 1, 3) + ls * 30 + LIGHT(x - 1, y - 1, z, 3) + LIGHT(x, y - 1, z - 1, 3)) / 5.0f / 15.0f;
					ls1 = (LIGHT(x + 1, y - 1, z + 1, 3) + ls * 30 + LIGHT(x + 1, y - 1, z, 3) + LIGHT(x, y - 1, z + 1, 3)) / 5.0f / 15.0f;
					ls2 = (LIGHT(x - 1, y - 1, z + 1, 3) + ls * 30 + LIGHT(x - 1, y - 1, z, 3) + LIGHT(x, y - 1, z + 1, 3)) / 5.0f / 15.0f;
					ls3 = (LIGHT(x + 1, y - 1, z - 1, 3) + ls * 30 + LIGHT(x + 1, y - 1, z, 3) + LIGHT(x, y - 1, z - 1, 3)) / 5.0f / 15.0f;
					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u1, v1, lr0, lg0, lb0, ls0);
					VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u2, v2, lr1, lg1, lb1, ls1);
					VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u1, v2, lr2, lg2, lb2, ls2);
					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u1, v1, lr0, lg0, lb0, ls0);
					VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u2, v1, lr3, lg3, lb3, ls3);
					VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u2, v2, lr1, lg1, lb1, ls1);
				}

				if (!IS_BLOCKED(x + 1, y, z)) {
					l = 0.95f;

					lr = LIGHT(x + 1, y, z, 0) / 15.0f;
					lg = LIGHT(x + 1, y, z, 1) / 15.0f;
					lb = LIGHT(x + 1, y, z, 2) / 15.0f;
					ls = LIGHT(x + 1, y, z, 3) / 15.0f;

					lr0 = (LIGHT(x + 1, y - 1, z - 1, 0) + lr * 30 + LIGHT(x + 1, y, z - 1, 0) + LIGHT(x + 1, y - 1, z, 0)) / 5.0f / 15.0f;
					lr1 = (LIGHT(x + 1, y + 1, z - 1, 0) + lr * 30 + LIGHT(x + 1, y, z - 1, 0) + LIGHT(x + 1, y + 1, z, 0)) / 5.0f / 15.0f;
					lr2 = (LIGHT(x + 1, y + 1, z + 1, 0) + lr * 30 + LIGHT(x + 1, y, z + 1, 0) + LIGHT(x + 1, y + 1, z, 0)) / 5.0f / 15.0f;
					lr3 = (LIGHT(x + 1, y - 1, z + 1, 0) + lr * 30 + LIGHT(x + 1, y, z + 1, 0) + LIGHT(x + 1, y - 1, z, 0)) / 5.0f / 15.0f;

					lg0 = (LIGHT(x + 1, y - 1, z - 1, 1) + lg * 30 + LIGHT(x + 1, y, z - 1, 1) + LIGHT(x + 1, y - 1, z, 1)) / 5.0f / 15.0f;
					lg1 = (LIGHT(x + 1, y + 1, z - 1, 1) + lg * 30 + LIGHT(x + 1, y, z - 1, 1) + LIGHT(x + 1, y + 1, z, 1)) / 5.0f / 15.0f;
					lg2 = (LIGHT(x + 1, y + 1, z + 1, 1) + lg * 30 + LIGHT(x + 1, y, z + 1, 1) + LIGHT(x + 1, y + 1, z, 1)) / 5.0f / 15.0f;
					lg3 = (LIGHT(x + 1, y - 1, z + 1, 1) + lg * 30 + LIGHT(x + 1, y, z + 1, 1) + LIGHT(x + 1, y - 1, z, 1)) / 5.0f / 15.0f;

					lb0 = (LIGHT(x + 1, y - 1, z - 1, 2) + lb * 30 + LIGHT(x + 1, y, z - 1, 2) + LIGHT(x + 1, y - 1, z, 2)) / 5.0f / 15.0f;
					lb1 = (LIGHT(x + 1, y + 1, z - 1, 2) + lb * 30 + LIGHT(x + 1, y, z - 1, 2) + LIGHT(x + 1, y + 1, z, 2)) / 5.0f / 15.0f;
					lb2 = (LIGHT(x + 1, y + 1, z + 1, 2) + lb * 30 + LIGHT(x + 1, y, z + 1, 2) + LIGHT(x + 1, y + 1, z, 2)) / 5.0f / 15.0f;
					lb3 = (LIGHT(x + 1, y - 1, z + 1, 2) + lb * 30 + LIGHT(x + 1, y, z + 1, 2) + LIGHT(x + 1, y - 1, z, 2)) / 5.0f / 15.0f;

					ls0 = (LIGHT(x + 1, y - 1, z - 1, 3) + ls * 30 + LIGHT(x + 1, y, z - 1, 3) + LIGHT(x + 1, y - 1, z, 3)) / 5.0f / 15.0f;
					ls1 = (LIGHT(x + 1, y + 1, z - 1, 3) + ls * 30 + LIGHT(x + 1, y, z - 1, 3) + LIGHT(x + 1, y + 1, z, 3)) / 5.0f / 15.0f;
					ls2 = (LIGHT(x + 1, y + 1, z + 1, 3) + ls * 30 + LIGHT(x + 1, y, z + 1, 3) + LIGHT(x + 1, y + 1, z, 3)) / 5.0f / 15.0f;
					ls3 = (LIGHT(x + 1, y - 1, z + 1, 3) + ls * 30 + LIGHT(x + 1, y, z + 1, 3) + LIGHT(x + 1, y - 1, z, 3)) / 5.0f / 15.0f;

					VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u2, v1, lr0, lg0, lb0, ls0);
					VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u2, v2, lr1, lg1, lb1, ls1);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u1, v2, lr2, lg2, lb2, ls2);
					VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u2, v1, lr0, lg0, lb0, ls0);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u1, v2, lr2, lg2, lb2, ls2);
					VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u1, v1, lr3, lg3, lb3, ls3);
				}
				if (!IS_BLOCKED(x - 1, y, z)) {
					l = 0.85f;
					lr = LIGHT(x - 1, y, z, 0) / 15.0f;
					lg = LIGHT(x - 1, y, z, 1) / 15.0f;
					lb = LIGHT(x - 1, y, z, 2) / 15.0f;
					ls = LIGHT(x - 1, y, z, 3) / 15.0f;

					lr0 = (LIGHT(x - 1, y - 1, z - 1, 0) + lr * 30 + LIGHT(x - 1, y, z - 1, 0) + LIGHT(x - 1, y - 1, z, 0)) / 5.0f / 15.0f;
					lr1 = (LIGHT(x - 1, y + 1, z + 1, 0) + lr * 30 + LIGHT(x - 1, y, z + 1, 0) + LIGHT(x - 1, y + 1, z, 0)) / 5.0f / 15.0f;
					lr2 = (LIGHT(x - 1, y + 1, z - 1, 0) + lr * 30 + LIGHT(x - 1, y, z - 1, 0) + LIGHT(x - 1, y + 1, z, 0)) / 5.0f / 15.0f;
					lr3 = (LIGHT(x - 1, y - 1, z + 1, 0) + lr * 30 + LIGHT(x - 1, y, z + 1, 0) + LIGHT(x - 1, y - 1, z, 0)) / 5.0f / 15.0f;

					lg0 = (LIGHT(x - 1, y - 1, z - 1, 1) + lg * 30 + LIGHT(x - 1, y, z - 1, 1) + LIGHT(x - 1, y - 1, z, 1)) / 5.0f / 15.0f;
					lg1 = (LIGHT(x - 1, y + 1, z + 1, 1) + lg * 30 + LIGHT(x - 1, y, z + 1, 1) + LIGHT(x - 1, y + 1, z, 1)) / 5.0f / 15.0f;
					lg2 = (LIGHT(x - 1, y + 1, z - 1, 1) + lg * 30 + LIGHT(x - 1, y, z - 1, 1) + LIGHT(x - 1, y + 1, z, 1)) / 5.0f / 15.0f;
					lg3 = (LIGHT(x - 1, y - 1, z + 1, 1) + lg * 30 + LIGHT(x - 1, y, z + 1, 1) + LIGHT(x - 1, y - 1, z, 1)) / 5.0f / 15.0f;

					lb0 = (LIGHT(x - 1, y - 1, z - 1, 2) + lb * 30 + LIGHT(x - 1, y, z - 1, 2) + LIGHT(x - 1, y - 1, z, 2)) / 5.0f / 15.0f;
					lb1 = (LIGHT(x - 1, y + 1, z + 1, 2) + lb * 30 + LIGHT(x - 1, y, z + 1, 2) + LIGHT(x - 1, y + 1, z, 2)) / 5.0f / 15.0f;
					lb2 = (LIGHT(x - 1, y + 1, z - 1, 2) + lb * 30 + LIGHT(x - 1, y, z - 1, 2) + LIGHT(x - 1, y + 1, z, 2)) / 5.0f / 15.0f;
					lb3 = (LIGHT(x - 1, y - 1, z + 1, 2) + lb * 30 + LIGHT(x - 1, y, z + 1, 2) + LIGHT(x - 1, y - 1, z, 2)) / 5.0f / 15.0f;

					ls0 = (LIGHT(x - 1, y - 1, z - 1, 3) + ls * 30 + LIGHT(x - 1, y, z - 1, 3) + LIGHT(x - 1, y - 1, z, 3)) / 5.0f / 15.0f;
					ls1 = (LIGHT(x - 1, y + 1, z + 1, 3) + ls * 30 + LIGHT(x - 1, y, z + 1, 3) + LIGHT(x - 1, y + 1, z, 3)) / 5.0f / 15.0f;
					ls2 = (LIGHT(x - 1, y + 1, z - 1, 3) + ls * 30 + LIGHT(x - 1, y, z - 1, 3) + LIGHT(x - 1, y + 1, z, 3)) / 5.0f / 15.0f;
					ls3 = (LIGHT(x - 1, y - 1, z + 1, 3) + ls * 30 + LIGHT(x - 1, y, z + 1, 3) + LIGHT(x - 1, y - 1, z, 3)) / 5.0f / 15.0f;
					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u1, v1, lr0, lg0, lb0, ls0);
					VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u2, v2, lr1, lg1, lb1, ls1);
					VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u1, v2, lr2, lg2, lb2, ls2);
					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u1, v1, lr0, lg0, lb0, ls0);
					VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u2, v1, lr3, lg3, lb3, ls3);
					VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u2, v2, lr1, lg1, lb1, ls1);
				}

				if (!IS_BLOCKED(x, y, z + 1)) {
					l = 0.9f;
					lr  = LIGHT(x, y, z + 1, 0) / 15.0f;
					lg  = LIGHT(x, y, z + 1, 1) / 15.0f;
					lb  = LIGHT(x, y, z + 1, 2) / 15.0f;
					ls  = LIGHT(x, y, z + 1, 3) / 15.0f;

					lr0 = l * (LIGHT(x - 1, y - 1, z + 1, 0) + lr * 30 + LIGHT(x, y - 1, z + 1, 0) + LIGHT(x - 1, y, z + 1, 0)) / 5.0f / 15.0f;
					lr1 = l * (LIGHT(x + 1, y + 1, z + 1, 0) + lr * 30 + LIGHT(x, y + 1, z + 1, 0) + LIGHT(x + 1, y, z + 1, 0)) / 5.0f / 15.0f;
					lr2 = l * (LIGHT(x - 1, y + 1, z + 1, 0) + lr * 30 + LIGHT(x, y + 1, z + 1, 0) + LIGHT(x - 1, y, z + 1, 0)) / 5.0f / 15.0f;
					lr3 = l * (LIGHT(x + 1, y - 1, z + 1, 0) + lr * 30 + LIGHT(x, y - 1, z + 1, 0) + LIGHT(x + 1, y, z + 1, 0)) / 5.0f / 15.0f;

					lg0 = l * (LIGHT(x - 1, y - 1, z + 1, 1) + lg * 30 + LIGHT(x, y - 1, z + 1, 1) + LIGHT(x - 1, y, z + 1, 1)) / 5.0f / 15.0f;
					lg1 = l * (LIGHT(x + 1, y + 1, z + 1, 1) + lg * 30 + LIGHT(x, y + 1, z + 1, 1) + LIGHT(x + 1, y, z + 1, 1)) / 5.0f / 15.0f;
					lg2 = l * (LIGHT(x - 1, y + 1, z + 1, 1) + lg * 30 + LIGHT(x, y + 1, z + 1, 1) + LIGHT(x - 1, y, z + 1, 1)) / 5.0f / 15.0f;
					lg3 = l * (LIGHT(x + 1, y - 1, z + 1, 1) + lg * 30 + LIGHT(x, y - 1, z + 1, 1) + LIGHT(x + 1, y, z + 1, 1)) / 5.0f / 15.0f;

					lb0 = l * (LIGHT(x - 1, y - 1, z + 1, 2) + lb * 30 + LIGHT(x, y - 1, z + 1, 2) + LIGHT(x - 1, y, z + 1, 2)) / 5.0f / 15.0f;
					lb1 = l * (LIGHT(x + 1, y + 1, z + 1, 2) + lb * 30 + LIGHT(x, y + 1, z + 1, 2) + LIGHT(x + 1, y, z + 1, 2)) / 5.0f / 15.0f;
					lb2 = l * (LIGHT(x - 1, y + 1, z + 1, 2) + lb * 30 + LIGHT(x, y + 1, z + 1, 2) + LIGHT(x - 1, y, z + 1, 2)) / 5.0f / 15.0f;
					lb3 = l * (LIGHT(x + 1, y - 1, z + 1, 2) + lb * 30 + LIGHT(x, y - 1, z + 1, 2) + LIGHT(x + 1, y, z + 1, 2)) / 5.0f / 15.0f;

					ls0 = l * (LIGHT(x - 1, y - 1, z + 1, 3) + ls * 30 + LIGHT(x, y - 1, z + 1, 3) + LIGHT(x - 1, y, z + 1, 3)) / 5.0f / 15.0f;
					ls1 = l * (LIGHT(x + 1, y + 1, z + 1, 3) + ls * 30 + LIGHT(x, y + 1, z + 1, 3) + LIGHT(x + 1, y, z + 1, 3)) / 5.0f / 15.0f;
					ls2 = l * (LIGHT(x - 1, y + 1, z + 1, 3) + ls * 30 + LIGHT(x, y + 1, z + 1, 3) + LIGHT(x - 1, y, z + 1, 3)) / 5.0f / 15.0f;
					ls3 = l * (LIGHT(x + 1, y - 1, z + 1, 3) + ls * 30 + LIGHT(x, y - 1, z + 1, 3) + LIGHT(x + 1, y, z + 1, 3)) / 5.0f / 15.0f;
					VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u1, v1, lr0, lg0, lb0, ls0);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u2, v2, lr1, lg1, lb1, ls1);
					VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u1, v2, lr2, lg2, lb2, ls2);
					VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u1, v1, lr0, lg0, lb0, ls0);
					VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u2, v1, lr2, lg2, lb2, ls2);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u2, v2, lr3, lg3, lb3, ls3);
				}
				if (!IS_BLOCKED(x, y, z - 1)) {
					l = 0.8f;
					lr = LIGHT(x, y, z - 1, 0) / 15.0f;
					lg = LIGHT(x, y, z - 1, 1) / 15.0f;
					lb = LIGHT(x, y, z - 1, 2) / 15.0f;
					ls = LIGHT(x, y, z - 1, 3) / 15.0f;

					lr0 = l * (LIGHT(x - 1, y - 1, z - 1, 0) + lr * 30 + LIGHT(x, y - 1, z - 1, 0) + LIGHT(x - 1, y, z - 1, 0)) / 5.0f / 15.0f;
					lr1 = l * (LIGHT(x - 1, y + 1, z - 1, 0) + lr * 30 + LIGHT(x, y + 1, z - 1, 0) + LIGHT(x - 1, y, z - 1, 0)) / 5.0f / 15.0f;
					lr2 = l * (LIGHT(x + 1, y + 1, z - 1, 0) + lr * 30 + LIGHT(x, y + 1, z - 1, 0) + LIGHT(x + 1, y, z - 1, 0)) / 5.0f / 15.0f;
					lr3 = l * (LIGHT(x + 1, y - 1, z - 1, 0) + lr * 30 + LIGHT(x, y - 1, z - 1, 0) + LIGHT(x + 1, y, z - 1, 0)) / 5.0f / 15.0f;

					lg0 = l * (LIGHT(x - 1, y - 1, z - 1, 1) + lg * 30 + LIGHT(x, y - 1, z - 1, 1) + LIGHT(x - 1, y, z - 1, 1)) / 5.0f / 15.0f;
					lg1 = l * (LIGHT(x - 1, y + 1, z - 1, 1) + lg * 30 + LIGHT(x, y + 1, z - 1, 1) + LIGHT(x - 1, y, z - 1, 1)) / 5.0f / 15.0f;
					lg2 = l * (LIGHT(x + 1, y + 1, z - 1, 1) + lg * 30 + LIGHT(x, y + 1, z - 1, 1) + LIGHT(x + 1, y, z - 1, 1)) / 5.0f / 15.0f;
					lg3 = l * (LIGHT(x + 1, y - 1, z - 1, 1) + lg * 30 + LIGHT(x, y - 1, z - 1, 1) + LIGHT(x + 1, y, z - 1, 1)) / 5.0f / 15.0f;

					lb0 = l * (LIGHT(x - 1, y - 1, z - 1, 2) + lb * 30 + LIGHT(x, y - 1, z - 1, 2) + LIGHT(x - 1, y, z - 1, 2)) / 5.0f / 15.0f;
					lb1 = l * (LIGHT(x - 1, y + 1, z - 1, 2) + lb * 30 + LIGHT(x, y + 1, z - 1, 2) + LIGHT(x - 1, y, z - 1, 2)) / 5.0f / 15.0f;
					lb2 = l * (LIGHT(x + 1, y + 1, z - 1, 2) + lb * 30 + LIGHT(x, y + 1, z - 1, 2) + LIGHT(x + 1, y, z - 1, 2)) / 5.0f / 15.0f;
					lb3 = l * (LIGHT(x + 1, y - 1, z - 1, 2) + lb * 30 + LIGHT(x, y - 1, z - 1, 2) + LIGHT(x + 1, y, z - 1, 2)) / 5.0f / 15.0f;

					ls0 = l * (LIGHT(x - 1, y - 1, z - 1, 3) + ls * 30 + LIGHT(x, y - 1, z - 1, 3) + LIGHT(x - 1, y, z - 1, 3)) / 5.0f / 15.0f;
					ls1 = l * (LIGHT(x - 1, y + 1, z - 1, 3) + ls * 30 + LIGHT(x, y + 1, z - 1, 3) + LIGHT(x - 1, y, z - 1, 3)) / 5.0f / 15.0f;
					ls2 = l * (LIGHT(x + 1, y + 1, z - 1, 3) + ls * 30 + LIGHT(x, y + 1, z - 1, 3) + LIGHT(x + 1, y, z - 1, 3)) / 5.0f / 15.0f;
					ls3 = l * (LIGHT(x + 1, y - 1, z - 1, 3) + ls * 30 + LIGHT(x, y - 1, z - 1, 3) + LIGHT(x + 1, y, z - 1, 3)) / 5.0f / 15.0f;

					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u2, v1, lr0, lg0, lb0, ls0);
					VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u2, v2, lr1, lg1, lb1, ls1);
					VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u1, v2, lr2, lg2, lb2, ls2);
					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u2, v1, lr0, lg0, lb0, ls0);
					VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u1, v2, lr3, lg3, lb3, ls3);
					VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u1, v1, lr1, lg1, lb1, ls1);
				}
			}
		}
	}
	return new Mesh(buffer, index / _VERTEX_SIZE, artts);
}

