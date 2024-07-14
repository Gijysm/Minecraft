#include "Mesh.h"
Mesh::Mesh(const float* buffer,
	size_t vertices,
	const int* atts) : vertices(vertices) {
	int vertex_size = 0;
	for (int i = 0; atts[i]; i++)
	{
		vertex_size += atts[i];
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_size * vertices,
		buffer, GL_STATIC_DRAW);

	int offset = 0;
	for (int i = 0; atts[i]; i++)
	{
		int size = atts[i];
		glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE,
			vertex_size * sizeof(float), (GLvoid*)(offset * sizeof(float)));
		glEnableVertexAttribArray(i);
		offset += size;
	}
	glBindVertexArray(0);
}

Mesh::~Mesh()
{	
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

}

void Mesh::draw(unsigned int primrtive)
{
	glBindVertexArray(VAO);
	glDrawArrays(primrtive, 0, vertices);
	glBindVertexArray(0);
}
