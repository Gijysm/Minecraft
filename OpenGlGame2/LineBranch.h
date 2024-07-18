#ifndef LINEBRANCH_H
#define LINEBRANCH_H
#include "lib.h"
#include "Defines.h"
#include "Chunk.h"
#include "Mesh.h"
#include "Voxel.h"
#define LB_VERTEX_SIZE (3 + 4)
class Mesh;
class Chunk;
class LineBranch
{
	Mesh* mesh;
	float* buffer;
	size_t index;
	size_t capacity;

public:
		LineBranch(size_t capacity);
		~LineBranch();
		void AddLine(float x1, float y1, float z1,
			float x2, float y2, float z2,
			float r, float g, float b, float a);
		void Cube(float x, float y, float z,
			float w, float h, float d, 
			float r, float g, float b, float a);
		void render();
		Mesh* GetMesh();
};

#endif