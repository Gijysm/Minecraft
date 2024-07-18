#ifndef MESH_H
#define MESH_H
#include "lib.h"
#include "Defines.h"
class Mesh
{
	unsigned int VAO, VBO;
	size_t vertices;
	size_t vertex_size;
public:

	Mesh(const float* buffer, 
		size_t vertices, 
		const int* atts);
	~Mesh();
	void reload(const float* buffer, size_t vertices);
	void draw(unsigned int primrtive);
};

#endif // !MESH_H