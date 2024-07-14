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
	for (int y = 0; y < _CHUNK_H; y++)
	{
		for (int z = 0; z < _CHUNK_D; z++)
		{
			for (int x = 0; x < _CHUNK_W; x++)
			{
				voxel vox = chunk->voxels[(y * _CHUNK_D + z) * _CHUNK_W + x];
				unsigned id= vox.id;
				if (!id)
				{
					continue;

				}
				float Light;
				if (!IS_BLOCKED(x, y + 1, z))
				{
					Light = 0.8;
					VERTEX(index, x - 0.5, y + 0.5, z - 0.5, 1, 0, Light);
					VERTEX(index, x - 0.5, y + 0.5, z + 0.5, 1, 1, Light);
					VERTEX(index, x + 0.5, y + 0.5, z + 0.5, 0, 1, Light);
					VERTEX(index, x - 0.5, y + 0.5, z - 0.5, 1, 0, Light);
					VERTEX(index, x + 0.5, y + 0.5, z + 0.5, 0, 1, Light);
					VERTEX(index, x + 0.5, y + 0.5, z - 0.5, 0, 0, Light);
				}												
				if (!IS_BLOCKED(x, y - 1, z))					
				{	
					Light = 0.2;
					VERTEX(index, x - 0.5, y - 0.5, z - 0.5, 1, 0, Light);
					VERTEX(index, x + 0.5, y - 0.5, z + 0.5, 0, 1, Light);
					VERTEX(index, x - 0.5, y - 0.5, z + 0.5, 1, 1, Light);
					VERTEX(index, x - 0.5, y - 0.5, z - 0.5, 1, 0, Light);
					VERTEX(index, x + 0.5, y - 0.5, z - 0.5, 0, 0, Light);
					VERTEX(index, x + 0.5, y - 0.5, z + 0.5, 0, 1, Light);
				}												 
				if (!IS_BLOCKED(x + 1, y, z))					 
				{	
					Light = 0.75;
					VERTEX(index, x + 0.5, y - 0.5, z - 0.5, 1, 0, Light);
					VERTEX(index, x + 0.5, y + 0.5, z + 0.5, 0, 1, Light);
					VERTEX(index, x + 0.5, y - 0.5, z + 0.5, 1, 1, Light);
					VERTEX(index, x + 0.5, y - 0.5, z - 0.5, 1, 0, Light);
					VERTEX(index, x + 0.5, y + 0.5, z - 0.5, 0, 0, Light);
					VERTEX(index, x + 0.5, y + 0.5, z + 0.5, 0, 1, Light);
				}												 
				if (!IS_BLOCKED(x - 1, y, z))					 
				{	
					Light = 0.75;
					VERTEX(index, x - 0.5, y - 0.5, z - 0.5, 1, 0, Light);
					VERTEX(index, x - 0.5, y - 0.5, z + 0.5, 1, 1, Light);
					VERTEX(index, x - 0.5, y + 0.5, z + 0.5, 0, 1, Light);
					VERTEX(index, x - 0.5, y - 0.5, z - 0.5, 1, 0, Light);
					VERTEX(index, x - 0.5, y + 0.5, z + 0.5, 0, 1, Light);
					VERTEX(index, x - 0.5, y + 0.5, z - 0.5, 0, 0, Light);
				}												
				if (!IS_BLOCKED(x, y, z + 1))					
				{	
					Light = 0.5;
					VERTEX(index, x - 0.5, y - 0.5, z + 0.5, 1, 0, Light);
					VERTEX(index, x + 0.5, y + 0.5, z + 0.5, 0, 1, Light);
					VERTEX(index, x - 0.5, y + 0.5, z + 0.5, 1, 1, Light);
					VERTEX(index, x - 0.5, y - 0.5, z + 0.5, 1, 0, Light);
					VERTEX(index, x + 0.5, y - 0.5, z + 0.5, 0, 0, Light);
					VERTEX(index, x + 0.5, y + 0.5, z + 0.5, 0, 1, Light);
				}												 
				if (!IS_BLOCKED(x, y, z - 1))					 
				{		
					Light = 0.5;
					VERTEX(index, x - 0.5, y - 0.5, z - 0.5, 1, 0, Light);
					VERTEX(index, x - 0.5, y + 0.5, z - 0.5, 1, 1, Light);
					VERTEX(index, x + 0.5, y + 0.5, z - 0.5, 0, 1, Light);
					VERTEX(index, x - 0.5, y - 0.5, z - 0.5, 1, 0, Light);
					VERTEX(index, x + 0.5, y + 0.5, z - 0.5, 0, 1, Light);
					VERTEX(index, x + 0.5, y - 0.5, z - 0.5, 0, 0, Light);
				}
			}
		}
	}
	return new Mesh(buffer, index / _VERTEX_SIZE, artts);
}
