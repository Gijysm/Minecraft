#ifndef SHADER_H
#define SHADER_H
#include "Defines.h"
#include "lib.h"
class Shader
{
public:
	uint id;
	Shader(uint id);
	~Shader();

	void use();
	void uniform_mat4(string name, glm::mat4 matrix);
};
extern Shader* load_shader(string fragment_file, string vertex_file);
#endif
