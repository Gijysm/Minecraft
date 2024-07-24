#ifndef MESH_H
#define MESH_H
#include "lib.h"
#include "Defines.h"
class Mesh
{
	unsigned int VAO, VBO;
	short unsigned int vertices;
	short unsigned int vertex_size;
public:

	Mesh(const float* buffer, 
		size_t vertices, 
		const int* atts);
	~Mesh();
	void reload(const float* buffer, const int& vertices);
	void draw(const short unsigned int& primrtive);
};

#endif // !MESH_H