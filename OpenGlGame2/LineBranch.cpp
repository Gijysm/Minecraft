#include "LineBranch.h"

LineBranch::LineBranch(size_t capacity) : capacity(capacity)
{
	int artts[] = { 3, 4, 0 };
	buffer = new float[capacity * LB_VERTEX_SIZE * 2];
	mesh = new Mesh(buffer, 0, artts);
	index = 0;
}

LineBranch::~LineBranch()
{
	delete[] buffer;
	delete mesh;
}

void LineBranch::AddLine(float x1, float y1, float z1, float x2, float y2, float z2,
	float r, float g, float b, float a)
{
	if (index >= capacity * LB_VERTEX_SIZE * 2)
		return;
	buffer[index] = x1;
	buffer[index + 1] = y1;
	buffer[index + 2] = z1;
	buffer[index + 3] = r;
	buffer[index + 4] = g;
	buffer[index + 5] = b;
	buffer[index + 6] = a;
	index += LB_VERTEX_SIZE;

	buffer[index] = x2;
	buffer[index + 1] = y2;
	buffer[index + 2] = z2;
	buffer[index + 3] = r;
	buffer[index + 4] = g;
	buffer[index + 5] = b;
	buffer[index + 6] = a;
	index += LB_VERTEX_SIZE;
}

void LineBranch::Cube(float x, float y, float z, float w, float h, float d, float r, float g, float b, float a)
{
	w *= 0.5f;
	h *= 0.5f;
	d *= 0.5f;
	AddLine(x - w, y - h, z - d, x + w, y - h, z - d, r, g, b, a);
	AddLine(x - w, y + h, z - d, x + w, y + h, z - d, r, g, b, a);
	AddLine(x - w, y - h, z + d, x + w, y - h, z + d, r, g, b, a);
	AddLine(x - w, y + h, z + d, x + w, y + h, z + d, r, g, b, a);

	AddLine(x - w, y - h, z - d, x - w, y + h, z - d, r, g, b, a);
	AddLine(x + w, y - h, z - d, x + w, y + h, z - d, r, g, b, a);
	AddLine(x - w, y - h, z + d, x - w, y + h, z + d, r, g, b, a);
	AddLine(x + w, y - h, z + d, x + w, y + h, z + d, r, g, b, a);

	AddLine(x - w, y - h, z - d, x - w, y - h, z + d, r, g, b, a);
	AddLine(x + w, y - h, z - d, x + w, y - h, z + d, r, g, b, a);
	AddLine(x - w, y + h, z - d, x - w, y + h, z + d, r, g, b, a);
	AddLine(x + w, y + h, z - d, x + w, y + h, z + d, r, g, b, a);
}

void LineBranch::render()
{
	if (index == 0)
		return;
	mesh->reload(buffer, index/ LB_VERTEX_SIZE);
	mesh->draw(GL_LINES);
	index = 0;
}
