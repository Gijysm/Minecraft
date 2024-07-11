#include "Shader.h"

Shader::Shader(uint id) : id(id)
{
}

Shader::~Shader()
{
	glDeleteProgram(id);
}

void Shader::use()
{
	glUseProgram(id);
	glUniform1i(glGetUniformLocation(id, "textureSampler"), 0); // Assuming texture unit 0
}

void Shader::uniform_mat4(string name, glm::mat4 matrix)
{
	GLuint transformLoc = glGetUniformLocation(id, name.c_str());
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
}

Shader* load_shader(string fragment_file, string vertex_file)
{
	string vertex_code;
	string fragment_code;
	ifstream vertex_file_stream;
	ifstream fragment_file_stream;
	vertex_file_stream.exceptions(ifstream::badbit);
	fragment_file_stream.exceptions(ifstream::badbit);
	try
	{
		vertex_file_stream.open(vertex_file);
		fragment_file_stream.open(fragment_file);

		stringstream vertex_stream, fragment_stream;
		vertex_stream << vertex_file_stream.rdbuf();
		fragment_stream << fragment_file_stream.rdbuf();

		vertex_file_stream.close();
		fragment_file_stream.close();

		vertex_code = vertex_stream.str();
		fragment_code = fragment_stream.str();
	}
	catch (ifstream::failure& e)
	{
		cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
	}
	const GLchar* vertex_code_c = vertex_code.c_str();
	const GLchar* fragment_code_c = fragment_code.c_str();
	GLuint vertex, fragment;
	GLint sucess;
	GLchar infoLog[512]; // Error message
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertex_code_c, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &sucess);
	if (!sucess)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);	
		cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
		cerr << infoLog << endl;
	}
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragment_code_c, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &sucess);
	if (!sucess)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
		cerr << infoLog << endl;
	}
	GLuint id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &sucess);
	if (!sucess)
	{
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		cerr << "ERROR::SHADER::PROGRAM_LICKING::COMPILATION_FAILED\n" << infoLog << endl;
		cerr << infoLog << endl;
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		glDeleteProgram(id);
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return new Shader(id);
}
